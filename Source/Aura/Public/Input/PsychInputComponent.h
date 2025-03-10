// Copyright Psych Ward 

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "PsychInputConfig.h"
#include "PsychInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UPsychInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UPsychInputConfig* InputConfig,
									UserClass* Object,
									PressedFuncType PressedFunc,
									ReleasedFuncType ReleasedFunc,
									HeldFuncType HeldFunc);
	
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UPsychInputComponent::BindAbilityActions(const UPsychInputConfig* InputConfig,
													UserClass* Object,
													PressedFuncType PressedFunc,
													ReleasedFuncType ReleasedFunc,
													HeldFuncType HeldFunc)
{
	check(InputConfig);

	for(const FPsychInputAction& Action : InputConfig->AbilityInputActions)
	{
		if(Action.InputAction && Action.InputTag.IsValid())
		{
			if(PressedFunc)
			{
				BindAction(Action.InputAction,ETriggerEvent::Started,Object,PressedFunc,Action.InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction,ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
			
			if(HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag); 
			}
		}
	}
	
}
