// Copyright Psych Ward 


#include "Actor/PsychEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/PsychAttributeSet.h"
#include "Components/SphereComponent.h"

// Sets default values
APsychEffectActor::APsychEffectActor()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void APsychEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO: Change this to apply a gameplay effect. For now, using const_cast as a hack!
	if(IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UPsychAttributeSet* PsychAttributeSet = Cast<UPsychAttributeSet>\
		(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UPsychAttributeSet::StaticClass()));

		UPsychAttributeSet* MutablePsychAttributeSet=const_cast<UPsychAttributeSet*>(PsychAttributeSet);
		MutablePsychAttributeSet->SetHealth(PsychAttributeSet->GetHealth() + 25.f);
		MutablePsychAttributeSet->SetMana(PsychAttributeSet->GetMana() + 10.f);
		Destroy();
	}
}

void APsychEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
 }


void APsychEffectActor::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&APsychEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this,&APsychEffectActor::EndOverlap);
}


