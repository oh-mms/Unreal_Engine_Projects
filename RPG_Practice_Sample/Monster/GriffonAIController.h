// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GriffonAIController.generated.h"

/**
 * 
 */

class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS()
class SINGLERPG_API AGriffonAIController : public AAIController
{
	GENERATED_BODY()

	virtual void OnPossess(class APawn* InPawn) override;

	UBehaviorTreeComponent* BehaviorComp;
	UBlackboardComponent* BlackboardComp;

	AGriffonAIController();

public:
	void SetEnemy(class APawn* InPawn);
};
