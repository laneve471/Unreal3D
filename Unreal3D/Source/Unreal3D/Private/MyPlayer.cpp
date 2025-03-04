// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "MyCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "MyAnimInstance.h"
#include "MyStatComponent.h"
#include "MyItem.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	_springArm->SetupAttachment(GetCapsuleComponent());
	_camera->SetupAttachment(_springArm);

	_springArm->TargetArmLength = 500.0f;
	_springArm->SetRelativeRotation(FRotator(-35.0f, 0.0f, 0.0f));
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent)
	{
		enhancedInputComponent->BindAction(_moveAction, ETriggerEvent::Triggered, this, &AMyPlayer::Move);
		enhancedInputComponent->BindAction(_lookAction, ETriggerEvent::Triggered, this, &AMyPlayer::Look);
		enhancedInputComponent->BindAction(_jumpAction, ETriggerEvent::Triggered, this, &AMyPlayer::MyJump);
		enhancedInputComponent->BindAction(_attackAction, ETriggerEvent::Triggered, this, &AMyPlayer::Attack);
		enhancedInputComponent->BindAction(_dropAction, ETriggerEvent::Triggered, this, &AMyPlayer::ItemDrop);
		enhancedInputComponent->BindAction(_dropAction, ETriggerEvent::Completed, this, &AMyPlayer::ItemDropEnd);
	}
}

void AMyPlayer::Move(const FInputActionValue& value)
{
	if (_isAttack) return;

	FVector2D moveVector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (moveVector.Length() > 0.01f)
		{
			FVector forward = GetActorForwardVector();
			FVector right = GetActorRightVector();

			_vertical = moveVector.Y;
			_horizontal = moveVector.X;

			AddMovementInput(forward, moveVector.Y);
			AddMovementInput(right, moveVector.X);
		}
	}
}

void AMyPlayer::Look(const FInputActionValue& value)
{
	FVector2D lookAxisVector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(lookAxisVector.X);
		AddControllerPitchInput(-lookAxisVector.Y);
	}
}

void AMyPlayer::MyJump(const FInputActionValue& value)
{
	if (_isAttack) return;

	bool isPress = value.Get<bool>();

	if (isPress)
	{
		ACharacter::Jump();
	}
}

void AMyPlayer::Attack(const FInputActionValue& value)
{
	if (_isAttack) return;

	bool isPress = value.Get<bool>();

	if (isPress)
	{
		Super::Attack();
	}
}

void AMyPlayer::ItemDrop(const FInputActionValue& value)
{
	if (_isAttack) return;

	bool isPress = value.Get<bool>();

	if (isPress && !_isPressed)
	{
		_isPressed = true;

		if (_items.Num() > 0)
		{
			AMyItem* dropItem = _items.Last();
			_items.Remove(dropItem);
			UE_LOG(LogTemp, Log, TEXT("Item count : %d"), _items.Num());

			FVector playerLocation = GetActorLocation();

			float dropRadius = 200.0f;
			FVector randomOffset = FMath::VRand() * FMath::FRandRange(50.0f, dropRadius);
			FVector dropLocation = playerLocation + randomOffset;
			dropLocation.Z = 40.0f;

			dropItem->SetActorLocation(dropLocation);
			dropItem->SetActorHiddenInGame(false);
			dropItem->SetActorEnableCollision(true);
		}
	}
}

void AMyPlayer::ItemDropEnd(const FInputActionValue& value)
{
	_isPressed = false;
	UE_LOG(LogTemp, Log, TEXT("_isPressed : %d"), _isPressed);
}

void AMyPlayer::AddExp(int32 amount)
{
	_statComponent->AddCurExp(amount);
}

void AMyPlayer::AddItem(AMyItem* item)
{
	_items.Add(item);
	UE_LOG(LogTemp, Log, TEXT("Item count : %d"), _items.Num());
}
