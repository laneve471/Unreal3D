// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStatComponent.h"

#include "MyGameInstance.h"

// Sets default values for this component's properties
UMyStatComponent::UMyStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	_gameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());

	_level = 1;
	LevelUp(_level);
}


// Called every frame
void UMyStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMyStatComponent::LevelUp(int32 level)
{
	auto statInfo = _gameInstance->GetStat_Level(level);
	_maxHp = statInfo.hp;
	_curHp = statInfo.hp;
	_atk = statInfo.atk;
	_curExp = 0;
	_requiredExp = statInfo.exp;
}

int32 UMyStatComponent::AddCurHp(float amount)
{
	int32 before = _curHp;
	
	_curHp += amount;

	if (_curHp < 0)
		_curHp = 0;
	if (_curHp > _maxHp)
		_curHp = _maxHp;

	float ratio = _curHp / (float)_maxHp;

	if (_hpChanged.IsBound())
		_hpChanged.Broadcast(ratio);

	auto actor = GetOwner();
	//UE_LOG(LogTemp, Warning, TEXT("Name : %s, HP: %d"), *actor->GetName(), _curHp);

	return before - _curHp;
}

void UMyStatComponent::AddCurExp(float amount)
{
	_curExp += amount;

	if (_curExp >= _requiredExp)
	{
		_level++;
		LevelUp(_level);
		UE_LOG(LogTemp, Log, TEXT("Level Up! Level : %d"), _level);
	}
}

