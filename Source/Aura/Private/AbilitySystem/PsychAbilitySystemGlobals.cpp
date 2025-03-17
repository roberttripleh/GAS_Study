// Copyright Psych Ward 


#include "AbilitySystem/PsychAbilitySystemGlobals.h"

#include "PsychAbilityTypes.h"

FGameplayEffectContext* UPsychAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FPsychGameplayEffectContext;
}
