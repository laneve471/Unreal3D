// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Service_FindTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"

#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"

#include "MyAIController.h"
#include "MyPlayer.h"

void UBT_Service_FindTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto curPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!IsValid(curPawn))
		return;

	FVector pos = curPawn->GetActorLocation();
	float sphereRadius = 500.0f;

	TArray<FOverlapResult> overlapResults;
	FCollisionQueryParams qParams(NAME_None, false, curPawn);

	bool result = GetWorld()->OverlapMultiByChannel(
		overlapResults,
		pos,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(sphereRadius),
		qParams
	);

	if (!result)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Player"), nullptr);
		DrawDebugSphere(GetWorld(), pos, sphereRadius, 30, FColor::Green, false, 0.3f);

		return;
	}
	
	for (auto& overlapResult : overlapResults)
	{
		auto player = Cast<AMyPlayer>(overlapResult.GetActor());
		if (IsValid(player))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Player"), player);
			DrawDebugSphere(GetWorld(), pos, sphereRadius, 30, FColor::Red, false, 0.3f);
			
			FRotator rot = (player->GetActorLocation() - curPawn->GetActorLocation()).GetSafeNormal().Rotation();
			curPawn->SetActorRotation(FMath::RInterpTo(curPawn->GetActorRotation(), rot, DeltaSeconds, 0.5f));
			
			return;
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Player"), nullptr);

	return;
}
