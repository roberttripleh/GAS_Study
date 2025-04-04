// Copyright Psych Ward 


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/PsychAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/PsychAbilitySystemComponent.h"
#include "Player/PsychPlayerState.h"

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
	
	APsychPlayerState* PsychPlayerState = CastChecked<APsychPlayerState>(PlayerState);
	PsychPlayerState->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 Points)
		{
			AttributePointsChangedDelegate.Broadcast(Points);
		}
	);
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UPsychAttributeSet* AS = CastChecked<UPsychAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for(auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	
	APsychPlayerState* PsychPlayerState = CastChecked<APsychPlayerState>(PlayerState);
	AttributePointsChangedDelegate.Broadcast(PsychPlayerState->GetAttributePoints());
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UPsychAbilitySystemComponent* PsychASC = CastChecked<UPsychAbilitySystemComponent>(AbilitySystemComponent);
	PsychASC->UpgradeAttribute(AttributeTag);
}

 void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& Attribute) const
{
	FPsychAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
