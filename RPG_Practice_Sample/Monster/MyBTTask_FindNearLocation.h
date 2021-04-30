// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MyBTTask_FindNearLocation.generated.h"

/**
 * 
 */
UCLASS()
class SINGLERPG_API UMyBTTask_FindNearLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	// Task는 ExecuteTask로 실행하게 되어있음.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* Node) override;
};
