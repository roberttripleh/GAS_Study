// Copyright Psych Ward 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PsychGameplayAbility.h"
#include "PsychDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UPsychDamageGameplayAbility : public UPsychGameplayAbility
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	TMap<FGameplayTag,FScalableFloat> DamageTypes;
};
