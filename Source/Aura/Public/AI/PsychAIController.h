// Copyright Psych Ward 

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PsychAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class AURA_API APsychAIController : public AAIController
{
	GENERATED_BODY()
public:
	APsychAIController();

protected:
	
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
