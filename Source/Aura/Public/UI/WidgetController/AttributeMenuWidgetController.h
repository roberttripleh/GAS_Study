// Copyright Psych Ward 

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/PsychWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"


class UAttributeInfo;
struct FPsychAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FPsychAttributeInfo&, Info);

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

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
