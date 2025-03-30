// Copyright Psych Ward 


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/PsychLogChannels.h"

FPsychAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FPsychAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogPsych, Error, TEXT("Cant find info for AbilityTag [%s] on AbilityInfo[%s]"),
			*AbilityTag.ToString(),*GetNameSafe(this));
	}

	return FPsychAbilityInfo();
}
