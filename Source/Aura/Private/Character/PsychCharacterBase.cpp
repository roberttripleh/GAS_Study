// Copyright Psych Ward 


#include "Character/PsychCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/PsychAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"

APsychCharacterBase::APsychCharacterBase()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon-> SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon-> SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

UAbilitySystemComponent* APsychCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


UAnimMontage* APsychCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void APsychCharacterBase::Die()
{
	Weapon->DetachFromComponent(
		FDetachmentTransformRules(
			EDetachmentRule::KeepWorld,
			true));
	
	MulticastHandleDeath_Implementation();
}

void APsychCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
}

void APsychCharacterBase::BeginPlay() 
{
	Super::BeginPlay();
	
}

FVector APsychCharacterBase::GetCombatSocketLocation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void APsychCharacterBase::InitAbilityActorInfo()
{
}

void APsychCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();

	ContextHandle.AddSourceObject(this);
	
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
													GameplayEffectClass, Level,ContextHandle);
	
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(
		*SpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void APsychCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes,1);
	ApplyEffectToSelf(DefaultSecondaryAttributes,1);
	ApplyEffectToSelf(DefaultVitalAttributes,1);
}

void APsychCharacterBase::AddCharacterAbilities()
{
	
	UPsychAbilitySystemComponent* PsychASC = CastChecked<UPsychAbilitySystemComponent>(AbilitySystemComponent);
	if(!HasAuthority()) return;

	PsychASC->AddCharacterAbilities(StartupAbilities);
	
}

void APsychCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(
			DissolveMaterialInstance,
			this);
		
		GetMesh()->SetMaterial(0, DynamicMatInst);
		
		StartDissolveTimeline(DynamicMatInst);
	}

	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(
			WeaponDissolveMaterialInstance,
			this);
		
		Weapon->SetMaterial(0, DynamicMatInst);
		
		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}



