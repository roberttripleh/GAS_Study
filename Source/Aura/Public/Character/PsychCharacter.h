// Copyright Psych Ward 

#pragma once

#include "CoreMinimal.h"
#include "Character/PsychCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "PsychCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API APsychCharacter : public APsychCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
public:
	APsychCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Player Interface **/
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	/** End Player Interface **/

	/** Combat Interface **/
	int32 GetPlayerLevel_Implementation() override;
	/** End Combat Interface **/
	
private:
	virtual void InitAbilityActorInfo() override;

};
