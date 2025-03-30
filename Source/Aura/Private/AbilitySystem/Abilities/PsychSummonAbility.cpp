// Copyright Psych Ward 


#include "AbilitySystem/Abilities/PsychSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UPsychSummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();

	//angle between each of the vectors i want to spawn minions along
	const float DeltaSpread = SpawnSpread / NumMinions;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread/2.f, FVector::UpVector);
	TArray<FVector> SpawnLocations;
	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		FVector ChosenSpawnLocation = Location +  Direction * FMath::FRandRange(MinSpawnDistance,MaxSpawnDistance);
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(
			Hit,
			ChosenSpawnLocation + FVector(0.f,0.f,400.f),
			ChosenSpawnLocation - FVector(0.f,0.f,400.f),
			ECC_Visibility);

		if(Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint;	
		}
		
		SpawnLocations.Add(ChosenSpawnLocation);
	}

	return SpawnLocations;
}

TSubclassOf<APawn> UPsychSummonAbility::GetRandomMinionClass()
{
	int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection];
}
