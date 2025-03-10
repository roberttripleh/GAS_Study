// Copyright Psych Ward 


#include "Input/PsychInputConfig.h"

const UInputAction* UPsychInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for (const FPsychInputAction& Action : AbilityInputActions)
	{
		if(Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]")
			,*InputTag.ToString(),*GetNameSafe(this));
	}
	
	return nullptr;
}
