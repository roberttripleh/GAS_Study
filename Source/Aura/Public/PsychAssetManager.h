// Copyright Psych Ward 

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "PsychAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UPsychAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	
	static UPsychAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
