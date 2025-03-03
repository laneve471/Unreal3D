// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyStatComponent.generated.h"

USTRUCT()
struct FMyStatData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 atk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 exp;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FHpChanged, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL3D_API UMyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	int32 GetCurHp() { return _curHp; }
	int32 GetAtk() { return _atk; }
	float GetSpeed() { return _speed; }

	void LevelUp(int32 level);

	int32 AddCurHp(float amount);
	void AddCurExp(float amount);

	bool IsDead() { return _curHp <= 0; }

	FHpChanged _hpChanged;

private:
	UPROPERTY()
	class UMyGameInstance* _gameInstance;


	UPROPERTY(VisibleAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	int32 _level = 0;

	UPROPERTY(VisibleAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	int32 _curHp = 100;

	UPROPERTY(VisibleAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	int32 _maxHp = 100;

	UPROPERTY(VisibleAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	int32 _atk = 10;

	UPROPERTY(VisibleAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	int32 _curExp = 0;

	UPROPERTY(VisibleAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	int32 _requiredExp = 10;

	UPROPERTY(VisibleAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float _speed = 10.0f;
};
