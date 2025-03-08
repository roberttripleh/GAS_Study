// Copyright Psych Ward 


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/PsychAttributeSet.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{

}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UPsychAttributeSet* AS = CastChecked<UPsychAttributeSet>(AttributeSet);
	
}
