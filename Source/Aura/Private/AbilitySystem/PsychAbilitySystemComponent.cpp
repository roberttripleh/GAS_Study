// Copyright Psych Ward 


#include "AbilitySystem/PsychAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "PsychGameplayTags.h"
#include "AbilitySystem/PsychAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/PsychGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/PsychLogChannels.h"
#include "Interaction/PlayerInterface.h"

void UPsychAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UPsychAbilitySystemComponent::ClientEffectApplied);
}

void UPsychAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		
		if(const UPsychGameplayAbility* PsychAbility = Cast<UPsychGameplayAbility>(AbilitySpec.Ability))
		{
			FGameplayTagContainer&  DynamicAbilityTags = AbilitySpec.GetDynamicSpecSourceTags();
			DynamicAbilityTags.AddTag(PsychAbility->StartupInputTag);
			DynamicAbilityTags.AddTag(FPsychGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}

	bStartupAbilitiesGiven = true;
	AbilitiesGiven.Broadcast();
}

void UPsychAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UPsychAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid()) return;

	for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		FGameplayTagContainer&  DynamicAbilityTags = AbilitySpec.GetDynamicSpecSourceTags();
		if(DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if(!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UPsychAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid()) return;

	for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		FGameplayTagContainer&  DynamicAbilityTags = AbilitySpec.GetDynamicSpecSourceTags();
		if(DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UPsychAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	/* Loops through activatable abilities using an active scope lock and execute the delegate that we've passed in
	 * 
	 * Will be executed for all activatable abilities
	 *
	 */
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogPsych, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag UPsychAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if(AbilitySpec.Ability)
	{
		for(FGameplayTag Tag : AbilitySpec.Ability->GetAssetTags())
		{
			if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UPsychAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if(Tag.MatchesTag((FGameplayTag::RequestGameplayTag(FName("InputTag")))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UPsychAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UPsychAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if(Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

void UPsychAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if(GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if(IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void UPsychAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

	if(GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
	}
}

void UPsychAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
{
	UAbilityInfo* AbilityInfo = UPsychAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FPsychAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		if(!Info.AbilityTag.IsValid()) continue;
		if(Level < Info.LevelRequirement) continue;
		if(GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			FGameplayTagContainer&  DynamicAbilityTags = AbilitySpec.GetDynamicSpecSourceTags();
			DynamicAbilityTags.AddTag(FPsychGameplayTags::Get().Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(Info.AbilityTag, FPsychGameplayTags::Get().Abilities_Status_Eligible);
		}
	}
}

void UPsychAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	if(!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGiven.Broadcast();
	}
}

void UPsychAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag);
}

void UPsychAbilitySystemComponent::ClientEffectApplied_Implementation(
												UAbilitySystemComponent* AbilitySystemComponent,
                                                 const FGameplayEffectSpec& EffectSpec,
                                                 FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}
	