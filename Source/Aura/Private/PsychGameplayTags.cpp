// Copyright Psych Ward 


#include "PsychGameplayTags.h"
#include "GameplayTagsManager.h"

FPsychGameplayTags FPsychGameplayTags::GameplayTags;

void FPsychGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"),
		FString("Reduces damage taken, improves Block Chance"));
}
