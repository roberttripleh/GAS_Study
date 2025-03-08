// Copyright Psych Ward 


#include "PsychAssetManager.h"
#include "PsychGameplayTags.h"

UPsychAssetManager& UPsychAssetManager::Get()
{
	check(GEngine);
	
	UPsychAssetManager* PsychAssetManager = Cast<UPsychAssetManager>(GEngine->AssetManager);

	return *PsychAssetManager;
	
}

void UPsychAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FPsychGameplayTags::InitializeNativeGameplayTags();
	
}
