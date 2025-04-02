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
	DOREPLIFETIME(APsychPlayerState, XP);
}

UAbilitySystemComponent* APsychPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APsychPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void APsychPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void APsychPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void APsychPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void APsychPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void APsychPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}
