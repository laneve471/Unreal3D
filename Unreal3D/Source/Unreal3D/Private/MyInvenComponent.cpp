// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInvenComponent.h"

// Sets default values for this component's properties
UMyInvenComponent::UMyInvenComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	_items.SetNum(9);
}


// Called when the game starts
void UMyInvenComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMyInvenComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMyInvenComponent::AddItem(FMyItemInfo itemInfo)
{
	FMyItemInfo temp;
	auto target = _items.FindByPredicate([temp](FMyItemInfo info)->bool
		{
			if (info.itemId == temp.itemId && info.type == temp.type)
				return true;
			return false;
		});

	if (target == nullptr)
		return;

	*target = itemInfo;

	int32 targetIndex = 0;
	int64 temp1 = (int64)target;
	int64 temp2 = (int64)(&_items[0]);
	targetIndex = (temp1 - temp2) / sizeof(int64);

	if(itemAddEvent.IsBound())
		itemAddEvent.Broadcast(targetIndex, *target);

	_itemCount++;
	//UE_LOG(LogTemp, Error, TEXT("ID : %d"), itemInfo.itemId);
}

FMyItemInfo UMyInvenComponent::DropItem()
{
	FMyItemInfo temp;
	auto target = _items.FindLastByPredicate([temp](FMyItemInfo info)->bool
		{
			if (info.itemId != temp.itemId || info.type != temp.type)
				return true;
			return false;
		});

	if (target == INDEX_NONE)
		return temp;

	FMyItemInfo oldItemInfo = _items[target];
	_items[target] = temp;

	if (itemDropEvent.IsBound())
		itemDropEvent.Broadcast(target, temp);

	_itemCount--;

	return oldItemInfo;
}

FMyItemInfo UMyInvenComponent::DropItem(int32 index)
{
	return FMyItemInfo();
}

