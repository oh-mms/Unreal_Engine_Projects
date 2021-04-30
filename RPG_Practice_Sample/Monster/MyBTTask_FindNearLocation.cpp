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
		const float SearchRadius = 300.f; // �κ� ���� ����

		FNavLocation RandomPt; // �ֺ��� ã�� ����
		const bool bFound = NavSys->GetRandomPointInNavigableRadius( // �ֺ��� ã�Ҵ���
									MyController->GetPawn()->GetActorLocation(), // ���� ��ġ
									SearchRadius, // ���� ����
									RandomPt); // �׺���̼� ��ġ

		if (bFound != NULL)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(GetSelectedBlackboardKey(), RandomPt.Location);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}