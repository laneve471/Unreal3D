// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"
#include "Animation/AnimMontage.h"

#include "MyCharacter.h"

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

void UMyAnimInstance::PlayAnimMontage()
{
	if (_animMontage == nullptr)
		return;

	if (!Montage_IsPlaying(_animMontage))
	{
		Montage_Play(_animMontage);
	}
}

void UMyAnimInstance::AnimNotify_Attack_Hit()
{
	_hitEvent.Broadcast();
}

void UMyAnimInstance::JumpToSection(int32 sectionIndex)
{
	FName sectionName = FName(*FString::Printf(TEXT("Section%d"), sectionIndex));
	Montage_JumpToSection(sectionName);
}
