// Copyright Psych Ward 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"


/**
 * PsychGameplayTags
 *
 * Singleton containing native Gameplay Tags
 * 
 */

struct FPsychGameplayTags
{
 public:
    static const FPsychGameplayTags& Get() {return GameplayTags;}
 
    static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Secondary_Armor;
 
 protected:

 private:
	static FPsychGameplayTags GameplayTags;
 
};
