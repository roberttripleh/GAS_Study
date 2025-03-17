// Copyright Psych Ward 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "PsychAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UPsychAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
