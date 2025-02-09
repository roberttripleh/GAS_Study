// Copyright Psych Ward 


#include "UI/WidgetController/OverlayWidgetController.h"

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
		PsychAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PsychAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PsychAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PsychAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
