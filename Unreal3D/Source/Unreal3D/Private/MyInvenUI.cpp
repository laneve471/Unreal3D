// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInvenUI.h"

#include "Components/UniformGridPanel.h"
#include "Components/Image.h"

#include "MyInvenButton.h"

bool UMyInvenUI::Initialize()
{
	Super::Initialize();

	_potionTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Graphics/UI/Items/Tex_seeds_06.Tex_seeds_06'"));
	_defaultTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Graphics/UI/Items/Tex_empty.Tex_empty'"));

	auto array = Grid->GetAllChildren();

	int index = 0;
	for (auto widget : array)
	{
		auto button = Cast<UMyInvenButton>(widget);
		if (button)
		{
			button->OnClicked.AddDynamic(button, &UMyInvenButton::SetCurIndex);
			button->_invenUI = this;
			button->_buttonIndex = index;
			index++;
		}

		auto image = Cast<UImage>(button->GetChildAt(0));
		if (image)
		{
			_slotImages.Add(image);
			_slotImages[index - 1]->SetBrushFromTexture(_defaultTexture);
		}
	}

	return true;
}

void UMyInvenUI::SetItem_Index(int32 index, FMyItemInfo info)
{
	if (info.itemId == 1 && info.type == MyItemType::POTION)
		_slotImages[index]->SetBrushFromTexture(_potionTexture);
	else
		_slotImages[index]->SetBrushFromTexture(_defaultTexture);
}