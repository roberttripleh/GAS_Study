// Copyright Psych Ward 


#include "Character/PsychCharacter.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/PsychAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/PsychPlayerController.h"
#include "Player/PsychPlayerState.h"
#include "UI/HUD/PsychHUD.h"


APsychCharacter::APsychCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;
	
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent ->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterClass = ECharacterClass::Elementalist;
}

void APsychCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	//init ability actor info for the server
	InitAbilityActorInfo();
	AddCharacterAbilities();

}

void APsychCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//init ability actor info for the client
	InitAbilityActorInfo();
}

void APsychCharacter::AddToXP_Implementation(int32 InXP)
{
	APsychPlayerState* PsychPlayerState = GetPlayerState<APsychPlayerState>();
	check(PsychPlayerState);
	PsychPlayerState->AddToXP(InXP);
}

void APsychCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void APsychCharacter::MulticastLevelUpParticles_Implementation() const
{
	if(IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();

		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();

		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		
		LevelUpNiagaraComponent->Activate(true);
	}
}

int32 APsychCharacter::GetXP_Implementation() const
{
	const APsychPlayerState* PsychPlayerState = GetPlayerState<APsychPlayerState>();
	check(PsychPlayerState);
	return PsychPlayerState->GetXP();
}

int32 APsychCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const APsychPlayerState* PsychPlayerState = GetPlayerState<APsychPlayerState>();
	check(PsychPlayerState);
	return PsychPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 APsychCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	const APsychPlayerState* PsychPlayerState = GetPlayerState<APsychPlayerState>();
	check(PsychPlayerState);
	return PsychPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointReward;
}

int32 APsychCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	const APsychPlayerState* PsychPlayerState = GetPlayerState<APsychPlayerState>();
	check(PsychPlayerState);
	return PsychPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointReward;
}

void APsychCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	APsychPlayerState* PsychPlayerState = GetPlayerState<APsychPlayerState>();
	check(PsychPlayerState);
	PsychPlayerState->AddToLevel(InPlayerLevel);
	if(UPsychAbilitySystemComponent* PsychASC = Cast<UPsychAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		PsychASC->UpdateAbilityStatuses(PsychPlayerState->GetPlayerLevel());
	}
}

void APsychCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	APsychPlayerState* PsychPlayerState = GetPlayerState<APsychPlayerState>();
	check(PsychPlayerState);
	PsychPlayerState->AddToAttributePoints(InAttributePoints);
}

void APsychCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	APsychPlayerState* PsychPlayerState = GetPlayerState<APsychPlayerState>();
	check(PsychPlayerState);
	PsychPlayerState->AddToSpellPoints(InSpellPoints);
}

int32 APsychCharacter::GetAttributePoints_Implementation() const
{
	const APsychPlayerState* PsychPlayerState = GetPlayerState<APsychPlayerState>();
	check(PsychPlayerState);
	return PsychPlayerState->GetAttributePoints();
}

int32 APsychCharacter::GetSpellPoints_Implementation() const
{
	const APsychPlayerState* PsychPlayerState = GetPlayerState<APsychPlayerState>();
	check(PsychPlayerState);
	return PsychPlayerState->GetSpellPoints();
}

int32 APsychCharacter::GetPlayerLevel_Implementation()
{
	const APsychPlayerState* PsychPlayerState = GetPlayerState<APsychPlayerState>();
	check(PsychPlayerState);
	
	return PsychPlayerState->GetPlayerLevel();
}

//init ability actor info
void APsychCharacter::InitAbilityActorInfo()
{
	APsychPlayerState* PsychPlayerState = GetPlayerState<APsychPlayerState>();
	check(PsychPlayerState)
	PsychPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(PsychPlayerState,this);
	Cast<UPsychAbilitySystemComponent>(PsychPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = PsychPlayerState->GetAbilitySystemComponent();
	AttributeSet = PsychPlayerState->GetAttributeSet();

	if(APsychPlayerController* PsychPlayerController = Cast<APsychPlayerController>(GetController()))
	{
		if (APsychHUD* PsychHUD = Cast<APsychHUD>(PsychPlayerController->GetHUD()))
		{
			PsychHUD->InitOverlay(PsychPlayerController,PsychPlayerState,AbilitySystemComponent,AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}
