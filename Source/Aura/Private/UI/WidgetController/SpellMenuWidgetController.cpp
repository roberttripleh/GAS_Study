// Copyright Psych Ward 


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/PsychAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/PsychPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadCastAbilityInfo();
	SpellPointsChanged.Broadcast(GetPsychPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetPsychASC()->AbilityStatusChanged.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
	{
			if(SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;
				
				bool bEnableSpendPoints = false;
				bool bEnableEquip = false;
				ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
				SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
			}
			
		if(AbilityInfo)
		{
			FPsychAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	}
	);

	GetPsychPS()->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 SpellPoints)
	{
			SpellPointsChanged.Broadcast(SpellPoints);
			CurrentSpellPoints = SpellPoints;
			
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
	}
	);
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const FPsychGameplayTags GameplayTags = FPsychGameplayTags::Get();	
	const int32 SpellPoints = GetPsychPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;	
 	
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetPsychASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetPsychASC()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquip);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
}

void USpellMenuWidgetController::ShouldEnableButtons(
	const FGameplayTag& AbilityStatus, int32 SpellPoints,
	bool& bShouldEnableSpellPointsButton,
	bool& bShouldEnableEquipButton)
{
	const FPsychGameplayTags GameplayTags = FPsychGameplayTags::Get();
 
	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
}
