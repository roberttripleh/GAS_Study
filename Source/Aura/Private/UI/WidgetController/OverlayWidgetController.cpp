// Copyright Psych Ward 


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/PsychAbilitySystemComponent.h"
#include "AbilitySystem/PsychAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/PsychPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UPsychAttributeSet* PsychAttributeSet = CastChecked<UPsychAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(PsychAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(PsychAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(PsychAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(PsychAttributeSet->GetMaxMana());

}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	APsychPlayerState* PsychPlayerState = CastChecked<APsychPlayerState>(PlayerState);
	
	PsychPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	
	const UPsychAttributeSet* PsychAttributeSet = CastChecked<UPsychAttributeSet>(AttributeSet);
	
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PsychAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PsychAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	PsychAttributeSet->GetManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	PsychAttributeSet->GetMaxManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
		);

	if(UPsychAbilitySystemComponent* PsychASC = Cast<UPsychAbilitySystemComponent>(AbilitySystemComponent))
	{
		if(PsychASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(PsychASC);
		}
		else
		{
			PsychASC->AbilitiesGiven.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
		}
		
		PsychASC->EffectAssetTags.AddLambda([this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				//"Message.HealthPotion".MatchesTag("Message") will return True
				//"Message".MatchesTag("Message.HealthPotion") will return False
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
			
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);	
				}
			
			} 
		}
		);
	}
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UPsychAbilitySystemComponent* PsychAbilitySystemComponent)
{
	//TODO: Get information about all given abilities, look up their ability info and broadcast it to widgets

	if(!PsychAbilitySystemComponent->bStartupAbilitiesGiven) return;

	/**	When on initialize startup abilities gets called, we create a broadcast delegate and bind a lambda to it
	 *	
	 *	That lambda creates a FPsychAbilityInfo which is set by calling find FindAbilityInfoForTag on our AbilityInfo data asset
	 *	
	 *	We need a tag for each of our ability specs and our lambda can receive an ability spec by const reference
	 *	This is why we created a function on the ASC to get the ability tag by looking up an ability spec
	 *	
	 *	Once we get that info we need to set its input tag, so we created another function on the ASC that can take in an ability spec
	 *	and look up its input tag from its dynamic gameplay tags
	 *	
	 *	Once we have that we set the input tag on that info
	 *	Once we have that info struct, we broadcast it so our widgets can receive it and do their own thing with that info
	 *	
	 *	After defining the lambda and binding it to our FForEachAbility deleagte, we call for each ability on the ASC,
	 *	passing in the delegate
	*/
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, PsychAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
	{
		FPsychAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(PsychAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = PsychAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	PsychAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
	const APsychPlayerState* PsychPlayerState = CastChecked<APsychPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = PsychPlayerState->LevelUpInfo;

	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo. Please fill out PsychPlayerState Blueprint"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;
		
		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}


