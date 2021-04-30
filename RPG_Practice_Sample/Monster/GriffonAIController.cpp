// Fill out your copyright notice in the Description page of Project Settings.


#include "GriffonAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "CGriffon.h"

AGriffonAIController::AGriffonAIController()
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void AGriffonAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ACGriffon* Bot = Cast<ACGriffon>(InPawn);

	if (Bot != nullptr && Bot->BotBehavior != nullptr)
	{
		if (Bot->BotBehavior->BlackboardAsset != nullptr)
		{
			BlackboardComp->InitializeBlackboard(*(Bot->BotBehavior->BlackboardAsset));
		}

		BehaviorComp->StartTree(*(Bot->BotBehavior));
	}
}

void AGriffonAIController::SetEnemy(APawn* InPawn)
{
	if (BlackboardComp != nullptr)
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Object>("TargetEnemy", InPawn);
	}
}