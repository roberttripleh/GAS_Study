// Copyright Psych Ward 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PsychGameplayAbility.h"
#include "PsychProjectileSpell.generated.h"

class APsychProjectile;
/**
 * 
 */
UCLASS()
class AURA_API UPsychProjectileSpell : public UPsychGameplayAbility
{
	GENERATED_BODY()
protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable,Category="Projectile")
	void SpawnProjectile();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<APsychProjectile> ProjectileClass;
	
};
