// Copyright Psych Ward 

#pragma once

#include "CoreMinimal.h"
#include "Character/PsychCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "PsychEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API APsychEnemy : public APsychCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	APsychEnemy();

	//Enemy Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//End Enemy interface

protected:
	virtual void BeginPlay() override;
};
