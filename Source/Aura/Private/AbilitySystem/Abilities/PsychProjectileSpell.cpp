// Copyright Psych Ward 


#include "AbilitySystem/Abilities/PsychProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/PsychProjectile.h"
#include "Interaction/CombatInterface.h"
#include "PsychGameplayTags.h"


void UPsychProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
	 
}

void UPsychProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsServer) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if(CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		//projectile rotation is parallel to the ground
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());
		
		APsychProjectile* Projectile = GetWorld()->SpawnActorDeferred<APsychProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
			GetAvatarActorFromActorInfo());

		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
			DamageEffectClass,
			GetAbilityLevel(),
			SourceASC->MakeEffectContext());

		const FPsychGameplayTags GameplayTags = FPsychGameplayTags::Get();
		const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
		
		
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
			SpecHandle,
			GameplayTags.Damage,
			ScaledDamage);
		
		Projectile->DamageEffectSpecHandle = SpecHandle;
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}
