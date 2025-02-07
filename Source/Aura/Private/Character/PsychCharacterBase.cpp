// Copyright Psych Ward 


#include "Character/PsychCharacterBase.h"


APsychCharacterBase::APsychCharacterBase()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon-> SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon-> SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

UAbilitySystemComponent* APsychCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void APsychCharacterBase::BeginPlay() 
{
	Super::BeginPlay();
	
}


