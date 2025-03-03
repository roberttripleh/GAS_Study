// Copyright Psych Ward 


#include "Player/PsychPlayerState.h"

#include "AbilitySystem/PsychAbilitySystemComponent.h"
#include "AbilitySystem/PsychAttributeSet.h"
#include "Net/UnrealNetwork.h"

APsychPlayerState::APsychPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UPsychAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent-> SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UPsychAttributeSet>("AttributeSet");
	
	SetNetUpdateFrequency(100.f);
}

void APsychPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APsychPlayerState,Level);
}

UAbilitySystemComponent* APsychPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APsychPlayerState::OnRep_Level(int32 OldLevel)
{
}
