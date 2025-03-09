// Copyright Psych Ward 


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/PsychAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "PsychGameplayTags.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UPsychAttributeSet* AS = CastChecked<UPsychAttributeSet>(AttributeSet);
	check(AttributeInfo);
	
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
		[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}

}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UPsychAttributeSet* AS = CastChecked<UPsychAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for(auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

 void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FPsychAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
