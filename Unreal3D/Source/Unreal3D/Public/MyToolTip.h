// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyItem.h"
#include "MyToolTip.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL3D_API UMyToolTip : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetItemInfo(FMyItemInfo info);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text;

private:
	UPROPERTY()
	UTexture2D* _defaultTexture;

	UPROPERTY()
	UTexture2D* _potionTexture;
};
