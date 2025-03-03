// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.h"
#include "MyMonster.generated.h"

UCLASS()
class UNREAL3D_API AMyMonster : public AMyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void Dead(class UAnimMontage* Montage, bool bInterrupted);
};
