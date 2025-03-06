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

#include "Blueprint/Userwidget.h"
#include "MyInvenUI.h"
#include "MyInvenComponent.h"

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

	static ConstructorHelpers::FClassFinder<UMyInvenUI> invenClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrints/BP_MyInvenUI.BP_MyInvenUI_C'"));
	if (invenClass.Succeeded())
	{
		_invenWidget = CreateWidget<UUserWidget>(GetWorld(), invenClass.Class);
	}

	_invenComponent = CreateDefaultSubobject<UMyInvenComponent>(TEXT("InvenComponent"));
}

void AMyPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	auto invenUI = Cast<UMyInvenUI>(_invenWidget);
	if (invenUI)
	{
		_invenComponent->itemAddEvent.AddUObject(invenUI, &UMyInvenUI::SetItem_Index);
		_invenComponent->itemDropEvent.AddUObject(invenUI, &UMyInvenUI::SetItem_Index);
	}
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (_invenWidget)
	{
		_invenWidget->AddToViewport();
	}
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

		auto dropItem = _invenComponent->DropItem();
		if (dropItem.itemId == -1)
			return;

		auto ItemBPClass = LoadClass<AMyItem>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrints/BP_MyItem.BP_MyItem_C'"));
		if (ItemBPClass)
		{
			FVector playerLocation = GetActorLocation();

			float dropRadius = 200.0f;
			FVector randomOffset = FMath::VRand() * FMath::FRandRange(100.0f, dropRadius);
			FVector dropLocation = playerLocation + randomOffset;
			dropLocation.Z = 40.0f;

			FActorSpawnParameters spawnParams;
			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			GetWorld()->SpawnActor<AMyItem>(ItemBPClass, dropLocation, FRotator::ZeroRotator, spawnParams);
		}
	}
}

void AMyPlayer::ItemDropEnd(const FInputActionValue& value)
{
	_isPressed = false;
}

void AMyPlayer::AddExp(int32 amount)
{
	_statComponent->AddCurExp(amount);
}

void AMyPlayer::AddItem(AMyItem* item)
{
	if (item && _invenComponent)
	{
		if (_invenComponent->IsFull())
			return;

		auto info = item->GetInfo();
		_invenComponent->AddItem(info);

		item->SetActorHiddenInGame(true);
		item->SetActorEnableCollision(false);
	}
}
