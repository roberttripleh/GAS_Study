// Copyright Psych Ward 

#pragma once

#include "CoreMinimal.h"
#include "Character/PsychCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "PsychEnemy.generated.h"


class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API APsychEnemy : public APsychCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	APsychEnemy();

	//Enemy Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//End Enemy interface

	//Combat Interface
	virtual int32 GetPlayerLevel() override;
	//End Combat Interface

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly )
	TObjectPtr<UWidgetComponent> HealthBar;

	

};
