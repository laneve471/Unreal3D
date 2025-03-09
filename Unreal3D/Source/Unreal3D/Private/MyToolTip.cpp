// Fill out your copyright notice in the Description page of Project Settings.


#include "MyToolTip.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

void UMyToolTip::SetItemInfo(FMyItemInfo info)
{
	if (info.itemId == -1 && info.type == MyItemType::NONE)
	{
		_defaultTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Graphics/UI/Items/Tex_empty.Tex_empty'"));
		//SetVisibility(ESlateVisibility::Hidden);
		Text->SetText(FText::FromString(TEXT("ItemType : NONE \nItemID : -1")));
		Image->SetBrushFromTexture(_defaultTexture);
	}
	if (info.itemId == 1 && info.type == MyItemType::POTION)
	{
		_potionTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Graphics/UI/Items/Tex_seeds_06_b.Tex_seeds_06_b'"));
		//SetVisibility(ESlateVisibility::Visible);
		Text->SetText(FText::FromString(TEXT("ItemType : POTION \nItemID : 1")));
		Image->SetBrushFromTexture(_potionTexture);
	}
}

