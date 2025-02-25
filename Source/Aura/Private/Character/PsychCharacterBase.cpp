// Copyright Psych Ward 


#include "Character/PsychCharacterBase.h"
#include "AbilitySystemComponent.h"

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

void APsychCharacterBase::InitAbilityActorInfo()
{
}

void APsychCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	
	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
													GameplayEffectClass, Level,ContextHandle);
	
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(
		*SpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void APsychCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes,1);
	ApplyEffectToSelf(DefaultSecondaryAttributes,1);
}



