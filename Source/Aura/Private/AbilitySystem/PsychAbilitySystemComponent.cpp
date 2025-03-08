// Copyright Psych Ward 


#include "AbilitySystem/PsychAbilitySystemComponent.h"

#include "PsychGameplayTags.h"

void UPsychAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UPsychAbilitySystemComponent::EffectApplied);

	const FPsychGameplayTags& GameplayTags = FPsychGameplayTags::Get();
	
}

void UPsychAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                 const FGameplayEffectSpec& EffectSpec,
                                                 FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
	

}
