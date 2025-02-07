// Copyright Psych Ward 


#include "AbilitySystem/PsychAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UPsychAttributeSet::UPsychAttributeSet()
{
	InitHealth(100.f);
	InitMana(100.f);
	InitMaxHealth(50.f);
	InitMaxMana(50.f);
}

void UPsychAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UPsychAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPsychAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPsychAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPsychAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UPsychAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPsychAttributeSet, Health, OldHealth);
}

void UPsychAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPsychAttributeSet, MaxHealth, OldMaxHealth);
}

void UPsychAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPsychAttributeSet, Mana, OldMana);
}

void UPsychAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPsychAttributeSet, MaxMana, OldMaxMana);
}
