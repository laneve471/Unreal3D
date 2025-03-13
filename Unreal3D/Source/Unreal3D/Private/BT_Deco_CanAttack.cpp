// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Deco_CanAttack.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "MyAIController.h"
#include "MyMonster.h"
#include "MyPlayer.h"

bool UBT_Deco_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!result)
		return result;

	auto curPawn = Cast<AMyMonster>(OwnerComp.GetAIOwner()->GetPawn());
	auto player = Cast<AMyPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Player")));

	if (!IsValid(curPawn) || !IsValid(player))
		return false;

	float distance = player->GetDistanceTo(curPawn);

	if (distance < curPawn->GetAttackRange())
		return true;

	return false;
}
