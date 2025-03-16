// Copyright Psych Ward 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PsychGameModeBase.generated.h"

class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class AURA_API APsychGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,Category="Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
};
