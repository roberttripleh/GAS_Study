// Copyright Psych Ward 


#include "Character/PsychCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PsychPlayerController.h"
#include "Player/PsychPlayerState.h"
#include "UI/HUD/PsychHUD.h"


APsychCharacter::APsychCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void APsychCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	//init ability actor info for the server
	InitAbilityActorInfo();


}

void APsychCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//init ability actor info for the client
	InitAbilityActorInfo();
}

//init ability actor info
void APsychCharacter::InitAbilityActorInfo()
{
	APsychPlayerState* PsychPlayerState = GetPlayerState<APsychPlayerState>();
	check(PsychPlayerState)
	PsychPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(PsychPlayerState,this);
	AbilitySystemComponent = PsychPlayerState->GetAbilitySystemComponent();
	AttributeSet = PsychPlayerState->GetAttributeSet();

	if(APsychPlayerController* PsychPlayerController = Cast<APsychPlayerController>(GetController()))
	{
		if (APsychHUD* PsychHUD = Cast<APsychHUD>(PsychPlayerController->GetHUD()))
		{
			PsychHUD->InitOverlay(PsychPlayerController,PsychPlayerState,AbilitySystemComponent,AttributeSet);
		}
	}
}
