// Copyright Psych Ward 


#include "UI/WidgetController/PsychWidgetController.h"

#include "AbilitySystem/PsychAbilitySystemComponent.h"
#include "AbilitySystem/PsychAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/PsychPlayerController.h"
#include "Player/PsychPlayerState.h"

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

void UPsychWidgetController::BroadCastAbilityInfo()
{
	if(!GetPsychASC()->bStartupAbilitiesGiven) return;

	/**	When on initialize startup abilities gets called, we create a broadcast delegate and bind a lambda to it
	 *	
	 *	That lambda creates a FPsychAbilityInfo which is set by calling find FindAbilityInfoForTag on our AbilityInfo data asset
	 *	
	 *	We need a tag for each of our ability specs and our lambda can receive an ability spec by const reference
	 *	This is why we created a function on the ASC to get the ability tag by looking up an ability spec
	 *	
	 *	Once we get that info we need to set its input tag, so we created another function on the ASC that can take in an ability spec
	 *	and look up its input tag from its dynamic gameplay tags
	 *	
	 *	Once we have that we set the input tag on that info
	 *	Once we have that info struct, we broadcast it so our widgets can receive it and do their own thing with that info
	 *	
	 *	After defining the lambda and binding it to our FForEachAbility deleagte, we call for each ability on the ASC,
	 *	passing in the delegate
	*/
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda(
		[this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FPsychAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(PsychAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = PsychAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = PsychAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	}
	);
	GetPsychASC()->ForEachAbility(BroadcastDelegate);
}

APsychPlayerController* UPsychWidgetController::GetPsychPC()
{
	if(PsychPlayerController == nullptr)
	{
		PsychPlayerController = Cast<APsychPlayerController>(PlayerController);
	}
	return PsychPlayerController;
}

APsychPlayerState* UPsychWidgetController::GetPsychPS()
{
	if(PsychPlayerState== nullptr)
	{
		PsychPlayerState = Cast<APsychPlayerState>(PlayerState);
	}
	return PsychPlayerState;
}

UPsychAbilitySystemComponent* UPsychWidgetController::GetPsychASC()
{
	if(PsychAbilitySystemComponent== nullptr)
	{
		PsychAbilitySystemComponent = Cast<UPsychAbilitySystemComponent>(AbilitySystemComponent);
	}
	return PsychAbilitySystemComponent;
}

UPsychAttributeSet* UPsychWidgetController::GetPsychAS()
{
	if(PsychAttributeSet== nullptr)
	{
		PsychAttributeSet = Cast<UPsychAttributeSet>(AttributeSet);
	}
	return PsychAttributeSet;
}
