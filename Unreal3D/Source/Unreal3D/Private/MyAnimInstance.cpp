// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

#include "MyCharacter.h"
#include "GameFramework/PawnMovementComponent.h"

UMyAnimInstance::UMyAnimInstance()
{
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	auto pawn = TryGetPawnOwner();

	AMyCharacter* character = Cast<AMyCharacter>(pawn);
	if (character)
	{
		_isFalling = character->GetMovementComponent()->IsFalling();
		_speed = character->GetVelocity().Size();
		_vertical = character->My_Vertical();
		_horizontal = character->My_Horizontal();
	}
}
