// Copyright Psych Ward 

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/PsychWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

/**
 * 
 */

UCLASS(BlueprintType,Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UPsychWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;
};
