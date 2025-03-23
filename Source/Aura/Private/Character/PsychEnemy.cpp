// Copyright Psych Ward 


#include "Character/PsychEnemy.h"

#include "AbilitySystem/PsychAbilitySystemComponent.h"
#include "AbilitySystem/PsychAbilitySystemLibrary.h"
#include "AbilitySystem/PsychAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/PsychUserWidget.h"
#include "PsychGameplayTags.h"
#include "AI/PsychAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APsychEnemy::APsychEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UPsychAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent-> SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	AttributeSet = CreateDefaultSubobject<UPsychAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("Health Bar");
	
	HealthBar->SetupAttachment(GetRootComponent());
}

void APsychEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(!HasAuthority()) return;
	PsychAIController = Cast<APsychAIController>(NewController);

	PsychAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	PsychAIController->RunBehaviorTree(BehaviorTree);
	
	PsychAIController->GetBlackboardComponent()->SetValueAsBool(
		FName("HitReacting"),
		false);
	PsychAIController->GetBlackboardComponent()->SetValueAsBool(
		FName("RangedAttacker"),
		CharacterClass != ECharacterClass::Warrior);

}

void APsychEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void APsychEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 APsychEnemy::GetPlayerLevel()
{
	return Level;
}

void APsychEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}

void APsychEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* APsychEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void APsychEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();

	if(HasAuthority())
	{
		UPsychAbilitySystemLibrary::GiveStartupAbilities(
			this,
			AbilitySystemComponent,
			CharacterClass);
	}

	if(UPsychUserWidget* PsychUserWidget = Cast<UPsychUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		PsychUserWidget->SetWidgetController(this);
	}
	
	if(const UPsychAttributeSet* PsychAS = Cast<UPsychAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			PsychAS->GetHealthAttribute())
		.AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
		);
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			PsychAS->GetMaxHealthAttribute())
		.AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
		);
		
		AbilitySystemComponent->RegisterGameplayTagEvent(
			FPsychGameplayTags::Get().Effects_HitReact,
			EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&APsychEnemy::HitReactTagChanged
			);
		
		OnHealthChanged.Broadcast(PsychAS->GetHealth());
		OnMaxHealthChanged.Broadcast(PsychAS->GetMaxHealth());
	}

}

void APsychEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	PsychAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	
}

void APsychEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UPsychAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if(HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

void APsychEnemy::InitializeDefaultAttributes() const
{
	UPsychAbilitySystemLibrary::InitializeDefaultAttributes(
		this,
		CharacterClass,
		Level,
		AbilitySystemComponent);
}
