// Copyright Psych Ward 

#pragma once

#include "CoreMinimal.h"
#include "Character/PsychCharacterBase.h"
#include "PsychCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API APsychCharacter : public APsychCharacterBase
{
	GENERATED_BODY()
public:
	APsychCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
private:
	void InitAbilityActorInfo();
};
