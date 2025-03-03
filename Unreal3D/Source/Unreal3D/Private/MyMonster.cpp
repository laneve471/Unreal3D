// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonster.h"

#include "MyAnimInstance.h"
#include "MyStatComponent.h"

// Sets default values
AMyMonster::AMyMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMyMonster::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	_statComponent->AddCurHp(-Damage);

	if (_statComponent->IsDead())
	{
		_animInstance->OnMontageEnded.AddDynamic(this, &AMyMonster::Dead);

		_curAttackSection = 4;
		_animInstance->PlayAnimMontage();
		_animInstance->JumpToSection(_curAttackSection);
	}

	return Damage;
}

void AMyMonster::Dead(class UAnimMontage* Montage, bool bInterrupted)
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

