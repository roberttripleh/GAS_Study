// Copyright Psych Ward 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PsychProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;


UCLASS()
class AURA_API APsychProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	APsychProjectile();

	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;	
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
						AActor* OtherActor,
						UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex,
						bool bFromSweep,
						const FHitResult& SweepResult);
	
private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
};
