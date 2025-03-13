// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Task_FindRandPos.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "MyAIController.h"

EBTNodeResult::Type UBT_Task_FindRandPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto curPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (curPawn->IsValidLowLevel() == false)
		return EBTNodeResult::Type::Failed;

	FVector pos = curPawn->GetActorLocation();

	auto naviSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (naviSystem->IsValidLowLevel() == false)
		return EBTNodeResult::Type::Failed;

	FNavLocation randLocation;
	if (naviSystem->GetRandomPointInNavigableRadius(pos, 300.0f, randLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("RandPos")), randLocation.Location);

		return EBTNodeResult::Type::Succeeded;
	}

	return EBTNodeResult::Type::Failed;
}
