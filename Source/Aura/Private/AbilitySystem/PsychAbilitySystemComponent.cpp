// Copyright Psych Ward 


#include "AbilitySystem/PsychAbilitySystemComponent.h"

#include "PsychGameplayTags.h"
#include "AbilitySystem/Abilities/PsychGameplayAbility.h"

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
			GiveAbility(AbilitySpec);
		}
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

void UPsychAbilitySystemComponent::ClientEffectApplied_Implementation(
												UAbilitySystemComponent* AbilitySystemComponent,
                                                 const FGameplayEffectSpec& EffectSpec,
                                                 FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}
