// Copyright Psych Ward 


#include "Player/PsychPlayerState.h"

#include "AbilitySystem/PsychAbilitySystemComponent.h"
#include "AbilitySystem/PsychAttributeSet.h"

APsychPlayerState::APsychPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UPsychAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent-> SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UPsychAttributeSet>("AttributeSet");
	
	SetNetUpdateFrequency(100.f);
}

UAbilitySystemComponent* APsychPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
