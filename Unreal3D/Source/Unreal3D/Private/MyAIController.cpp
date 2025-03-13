// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"

#include "NavigationSystem.h"

#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AMyAIController::AMyAIController()
{
}

void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UBlackboardComponent* temp = Blackboard;
	if (UseBlackboard(_blackBoard, temp))
	{
		if (RunBehaviorTree(_behaviorTree))
		{

		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("BT Failed."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BB Failed."));
	}
}

void AMyAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
