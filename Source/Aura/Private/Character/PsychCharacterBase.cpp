// Copyright Psych Ward 


#include "Character/PsychCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "PsychGameplayTags.h"
#include "AbilitySystem/PsychAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

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

TArray<FTaggedMontage> APsychCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

UNiagaraSystem* APsychCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage APsychCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag == MontageTag)
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

int32 APsychCharacterBase::GetMinionCount_Implementation()
{
	return MinionCount; 
}

void APsychCharacterBase::IncrementMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
}

void APsychCharacterBase::MulticastHandleDeath_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(),GetActorRotation());
	
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	bDead = true;
}

void APsychCharacterBase::BeginPlay() 
{
	Super::BeginPlay();
	
}

FVector APsychCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{

	// Q: Let’s say you want to have multiple attack sections in the same montage.
	// How could you make sure you get the correct socket for each attack?
	
	// A: Simply use a Gameplay Tag that represents the socket, not the montage.
	
	// for a more data driven approach, this can be a TMap<MontageTags,FName> for the socket location
	// this will then become an algorithm that will look up the TMap
	const FPsychGameplayTags& GameplayTags = FPsychGameplayTags::Get();
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}

	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}

	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	
	return FVector();
}

bool APsychCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* APsychCharacterBase::GetAvatar_Implementation()
{
	return this;
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



