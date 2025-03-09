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

FMyItemInfo UMyInvenComponent::GetItemInfo_Index(int32 index)
{
	if (index < 0 || index >= _items.Num())
		return FMyItemInfo();

	if (_items[index] == nullptr)
		return FMyItemInfo();

	return _items[index]->GetInfo();
}

void UMyInvenComponent::AddItem(AMyItem* item)
{
	auto target = _items.IndexOfByPredicate([](AMyItem* item)->bool
		{
			return item == nullptr;
		});

	if (target == INDEX_NONE)
		return;

	_items[target] = item;

	if(_itemAddDropEvent.IsBound())
		_itemAddDropEvent.Broadcast(target, item->GetInfo());
}

AMyItem* UMyInvenComponent::DropItem()
{
	auto target = _items.FindLastByPredicate([](AMyItem* item)->bool
		{
			return item != nullptr;
		});

	if (target == INDEX_NONE)
		return nullptr;

	AMyItem* dropItemInfo = _items[target];
	_items[target] = nullptr;

	if (_itemAddDropEvent.IsBound())
		_itemAddDropEvent.Broadcast(target, FMyItemInfo());

	return dropItemInfo;
}

AMyItem* UMyInvenComponent::DropItem(int32 index)
{
	if (index >= _items.Num() || index < 0)
		return nullptr;

	if (_items[index] == nullptr)
		return nullptr;

	AMyItem* dropItemInfo = _items[index];
	_items[index] = nullptr;

	if (_itemAddDropEvent.IsBound())
		_itemAddDropEvent.Broadcast(index, FMyItemInfo());

	return dropItemInfo;
}

bool UMyInvenComponent::IsFull()
{
	auto target = _items.IndexOfByPredicate([](AMyItem* item)->bool
		{
			return item == nullptr;
		});

	return target == INDEX_NONE;
}

