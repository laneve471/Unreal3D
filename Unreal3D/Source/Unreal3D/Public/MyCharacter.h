// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class UNREAL3D_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Attack();

	UFUNCTION()
	void AttackEnd(class UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void Attack_Hit();

	void AddHp(float Amount);
	void SubtractHp(float Amount);

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	float My_Vertical() { return _vertical; }
	float My_Horizontal() { return _horizontal; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	class UMyStatComponent* _statComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* _hpBarWidget;

	UPROPERTY()
	class UMyAnimInstance* _animInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	bool _isAttack;


	int32 _curAttackSection = 0;

	float _vertical = 0.0f;
	float _horizontal = 0.0f;
};
