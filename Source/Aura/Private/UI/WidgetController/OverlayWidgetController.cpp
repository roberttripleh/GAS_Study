// Copyright Psych Ward 


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/PsychAbilitySystemComponent.h"
#include "AbilitySystem/PsychAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UPsychAttributeSet* PsychAttributeSet = CastChecked<UPsychAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(PsychAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(PsychAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(PsychAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(PsychAttributeSet->GetMaxMana());

}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UPsychAttributeSet* PsychAttributeSet = CastChecked<UPsychAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PsychAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PsychAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	PsychAttributeSet->GetManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	PsychAttributeSet->GetMaxManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
		);

	Cast<UPsychAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
	[this](const FGameplayTagContainer& AssetTags)
	{
		for (const FGameplayTag& Tag : AssetTags)
		{
			//"Message.HealthPotion".MatchesTag("Message") will return True
			//"Message".MatchesTag("Message.HealthPotion") will return False
			FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
			
			if (Tag.MatchesTag(MessageTag))
			{
				const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
				MessageWidgetRowDelegate.Broadcast(*Row);	
			}
			
		} 
	}
	);
}


