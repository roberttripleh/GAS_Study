// Copyright Psych Ward 


#include "UI/WidgetController/PsychWidgetController.h"

void UPsychWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UPsychWidgetController::BroadcastInitialValues()
{
}

void UPsychWidgetController::BindCallbacksToDependencies()
{
}
