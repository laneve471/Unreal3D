// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInvenUI.h"

#include "Components/UniformGridPanel.h"
#include "Components/Image.h"

#include "MyInvenButton.h"
#include "MyToolTip.h"

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
			auto toolTipClass = LoadObject<UClass>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrints/BP_MyToolTip.BP_MyToolTip_C'"));

			if (toolTipClass)
			{
				auto toolTip = CreateWidget<UMyToolTip>(GetWorld(), toolTipClass);

				button->_toolTip = toolTip;
				button->SetToolTip(nullptr);
			}
			button->OnClicked.AddDynamic(button, &UMyInvenButton::SetCurIndex);
			button->_invenUI = this;
			button->_buttonIndex = index;

			_buttons.Add(button);
			index++;
		}

		auto image = Cast<UImage>(button->GetChildAt(0));
		if (image)
		{
			_slotImages.Add(image);
		}
	}

	return true;
}

void UMyInvenUI::SetItem_Index(int32 index, FMyItemInfo info)
{
	if (info.itemId == -1 && info.type == MyItemType::NONE)
	{
		_slotImages[index]->SetBrushFromTexture(_defaultTexture);
		_buttons[index]->_toolTip->SetItemInfo(info);
		_buttons[index]->SetToolTip(nullptr);
	}

	if (info.itemId == 1 && info.type == MyItemType::POTION)
	{
		_slotImages[index]->SetBrushFromTexture(_potionTexture);
		_buttons[index]->_toolTip->SetItemInfo(info);
		_buttons[index]->ShowToolTip();
	}
}