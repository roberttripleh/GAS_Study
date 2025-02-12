// Copyright Psych Ward 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PsychAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UPsychAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void AbilityActorInfoSet();
	
protected:
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec,FActiveGameplayEffectHandle ActiveEffectHandle);
	
};
