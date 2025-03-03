// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/DamageEvents.h"

#include "MyAnimInstance.h"
#include "MyStatComponent.h"
#include "MyHpBar.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	_statComponent = CreateDefaultSubobject<UMyStatComponent>(TEXT("Stat"));

	_hpBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	_hpBarWidget->SetupAttachment(GetMesh());
	_hpBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UMyHpBar> hpBarClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrints/BP_MyHpBar.BP_MyHpBar_C'"));
	if (hpBarClass.Succeeded())
	{
		_hpBarWidget->SetWidgetClass(hpBarClass.Class);
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	_animInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (_animInstance == nullptr)
		UE_LOG(LogTemp, Error, TEXT("AnimInstance did not exist."));

	_animInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::AttackEnd);
	_animInstance->_hitEvent.AddDynamic(this, &AMyCharacter::Attack_Hit);

	auto hpBar = Cast<UMyHpBar>(_hpBarWidget->GetWidget());
	if (hpBar)
	{
		_statComponent->_hpChanged.AddUObject(hpBar, &UMyHpBar::SetHpBarValue);
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto playerController = GetWorld()->GetFirstPlayerController();
	if (playerController)
	{
		FVector playerLocation = playerController->GetPawn()->GetActorLocation();
		float distance = FVector::Distance(GetActorLocation(), playerLocation);

		if (distance > 1000.0f)
		{
			_hpBarWidget->SetVisibility(false);
		}
		else
		{
			_hpBarWidget->SetVisibility(true);
		}
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyCharacter::Attack()
{
	_isAttack = true;

	_curAttackSection = (_curAttackSection + 1) % 3;
	_animInstance->PlayAnimMontage();
	_animInstance->JumpToSection(_curAttackSection + 1);
}

void AMyCharacter::AttackEnd(UAnimMontage* Montage, bool bInterrupted)
{
	_isAttack = false;
}

void AMyCharacter::Attack_Hit()
{
	FHitResult hitResult;
	FCollisionQueryParams params(NAME_None, false, this);

	float attackRange = 500.0f;
	float attackRadius = 100.0f;

	FVector forward = GetActorForwardVector();
	FQuat quat = FQuat::FindBetweenVectors(FVector(0, 0, 1), forward);

	FVector center = GetActorLocation() + forward * attackRange * 0.5f;

	bool bResult = GetWorld()->SweepSingleByChannel
	(
		OUT hitResult,
		center,
		center,
		quat,
		ECC_GameTraceChannel2,
		FCollisionShape::MakeCapsule(attackRadius, attackRange * 0.5f),
		params
	);

	FColor drawColor = FColor::Green;

	if (bResult && hitResult.GetActor()->IsValidLowLevel())
	{
		drawColor = FColor::Red;
		auto victim = Cast<AMyCharacter>(hitResult.GetActor());

		if (victim)
		{
			FDamageEvent damageEvent = FDamageEvent();

			victim->TakeDamage(_statComponent->GetAtk(), damageEvent, GetController(), this);
		}
	}

	DrawDebugCapsule(GetWorld(), center,
		attackRange * 0.5f, attackRadius, quat, drawColor, false, 1.0f);
}

void AMyCharacter::AddHp(float Amount)
{
	_statComponent->AddCurHp(Amount);
}

void AMyCharacter::SubtractHp(float Amount)
{
	_statComponent->AddCurHp(-Amount);
}

float AMyCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	_statComponent->AddCurHp(-Damage);

	return Damage;
}

