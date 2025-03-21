// Copyright Psych Ward 

#pragma once

#include "CoreMinimal.h"
#include "PsychDamageGameplayAbility.h"
#include "PsychProjectileSpell.generated.h"

class APsychProjectile;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class AURA_API UPsychProjectileSpell : public UPsychDamageGameplayAbility
{
	GENERATED_BODY()
protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable,Category="Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<APsychProjectile> ProjectileClass;
	
};
