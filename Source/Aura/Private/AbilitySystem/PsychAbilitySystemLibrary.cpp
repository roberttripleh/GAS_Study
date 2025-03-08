// Copyright Psych Ward 


#include "AbilitySystem/PsychAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PsychPlayerState.h"
#include "UI/HUD/PsychHUD.h"
#include "UI/WidgetController/PsychWidgetController.h"


UOverlayWidgetController* UPsychAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if(APsychHUD* PsychHUD = Cast<APsychHUD>(PC->GetHUD()))
		{
			APsychPlayerState* PS = PC->GetPlayerState<APsychPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
			
			return PsychHUD->GetOverlayWidgetController(WidgetControllerParams);
			
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UPsychAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if(APsychHUD* PsychHUD = Cast<APsychHUD>(PC->GetHUD()))
		{
			APsychPlayerState* PS = PC->GetPlayerState<APsychPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
			
			return PsychHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
			
		}
	}

	return nullptr;
}
