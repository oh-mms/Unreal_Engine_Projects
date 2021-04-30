// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_FindNearLocation.h"
#include "BehaviorTree/BTNode.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "GriffonAIController.h"

EBTNodeResult::Type UMyBTTask_FindNearLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* Node)
{
	const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	AGriffonAIController* MyController = Cast<AGriffonAIController>(OwnerComp.GetAIOwner());

	if (NavSys != nullptr &&
		MyController != nullptr &&
		MyController->GetPawn() != nullptr)
	{
		const float SearchRadius = 300.f; // 로봇 정찰 구역

		FNavLocation RandomPt; // 주변을 찾을 변수
		const bool bFound = NavSys->GetRandomPointInNavigableRadius( // 주변을 찾았는지
									MyController->GetPawn()->GetActorLocation(), // 폰의 위치
									SearchRadius, // 정찰 구역
									RandomPt); // 네비게이션 위치

		if (bFound != NULL)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(GetSelectedBlackboardKey(), RandomPt.Location);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}