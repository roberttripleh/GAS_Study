// Copyright Psych Ward 


#include "Character/PsychEnemy.h"

#include "AbilitySystem/PsychAbilitySystemComponent.h"
#include "AbilitySystem/PsychAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/PsychUserWidget.h"

APsychEnemy::APsychEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UPsychAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent-> SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UPsychAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("Health Bar");
	
	HealthBar->SetupAttachment(GetRootComponent());
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

void APsychEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();

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

		OnHealthChanged.Broadcast(PsychAS->GetHealth());
		OnMaxHealthChanged.Broadcast(PsychAS->GetMaxHealth());
	}

}

void APsychEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UPsychAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();
}
