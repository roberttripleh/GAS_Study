// Copyright Psych Ward 


#include "AbilitySystem/PsychAbilitySystemLibrary.h"

#include "Game/PsychGameModeBase.h"
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

void UPsychAbilitySystemLibrary::InitializeDefaultAttributes(
	const UObject* WorldContextObject,
	ECharacterClass CharacterClass,
	float Level,
	UAbilitySystemComponent* ASC)
{
	APsychGameModeBase* PsychGameMode = Cast<APsychGameModeBase>(
		UGameplayStatics::GetGameMode(WorldContextObject));

	if(PsychGameMode == nullptr) return;

	AActor* AvatarActor = ASC->GetAvatarActor();
	
	UCharacterClassInfo* CharacterClassInfo = PsychGameMode->CharacterClassInfo;
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	// Primary Attributes
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(
		ClassDefaultInfo.PrimaryAttributes,
		Level,
		PrimaryAttributesContextHandle);
	
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	// Secondary Attributes
	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);

	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(
	CharacterClassInfo->SecondaryAttributes,
	Level,
	SecondaryAttributesContextHandle);
	
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	// Vital Attributes
	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(
	CharacterClassInfo->VitalAttributes,
	Level,
	VitalAttributesContextHandle);
	
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());	
	
}
