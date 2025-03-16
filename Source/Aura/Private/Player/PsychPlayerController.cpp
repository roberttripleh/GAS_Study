// Copyright Psych Ward 


#include "Player/PsychPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "PsychGameplayTags.h"
#include "AbilitySystem/PsychAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/PsychInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextComponent.h"

APsychPlayerController::APsychPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void APsychPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
}

void APsychPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter)
{
	if(IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(
			TargetCharacter->GetRootComponent(),
			FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount);
	}
}

void APsychPlayerController::AutoRun()
{
	if(!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
													ControlledPawn->GetActorLocation(),
													ESplineCoordinateSpace::World);
		
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(
													LocationOnSpline,
													ESplineCoordinateSpace::World);
		
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void APsychPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility,false,CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();
	
	if(LastActor != ThisActor)
	{
		if(LastActor) LastActor->UnHighlightActor();
		if(ThisActor) ThisActor->HighlightActor();
	}
}

void APsychPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(InputTag.MatchesTagExact(FPsychGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void APsychPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if(!InputTag.MatchesTagExact(FPsychGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}

	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
	
	if (!bTargeting && !bShiftKeyDown)
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if(UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
																this,
																ControlledPawn->GetActorLocation(),
																CachedDestination))
			{
				Spline->ClearSplinePoints();
				for(const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}

				if(NavPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() -1 ];
					bAutoRunning = true;
				}
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void APsychPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if(!InputTag.MatchesTagExact(FPsychGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}

	if (bTargeting || bShiftKeyDown)
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		if(CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;

		if(APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation())
																						.GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UPsychAbilitySystemComponent* APsychPlayerController::GetASC()
{
	if(PsychAbilitySystemComponent == nullptr)
	{
		PsychAbilitySystemComponent = Cast<UPsychAbilitySystemComponent>
			(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
			GetPawn<APawn>()));
	}
	return PsychAbilitySystemComponent;
}


void APsychPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(PsychContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	
	if(Subsystem)
	{
		Subsystem->AddMappingContext(PsychContext, 0);	
	}
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void APsychPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UPsychInputComponent* PsychInputComponent = CastChecked<UPsychInputComponent>(InputComponent);
	
	PsychInputComponent->BindAction(
		MoveAction, ETriggerEvent::Triggered,
		this, &APsychPlayerController::Move);

	PsychInputComponent->BindAction(
		ShiftAction,ETriggerEvent::Started,
		this,
		&APsychPlayerController::ShiftPressed);

	PsychInputComponent->BindAction(
	ShiftAction,ETriggerEvent::Completed,
	this,
	&APsychPlayerController::ShiftReleased);

	PsychInputComponent->BindAbilityActions(
		InputConfig,
	this,
		&ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased,
		&ThisClass::AbilityInputTagHeld);
}


void APsychPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}


