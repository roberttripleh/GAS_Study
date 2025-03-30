// Copyright Psych Ward 


#include "AbilitySystem/Data/AttributeInfo.h"

#include "Aura/PsychLogChannels.h"

FPsychAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{

	for (const FPsychAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogPsych, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s].")
			,*AttributeTag.ToString(),*GetNameSafe(this))
	}

	return FPsychAttributeInfo();
	
}
