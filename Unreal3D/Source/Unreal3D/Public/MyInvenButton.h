// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "MyInvenButton.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL3D_API UMyInvenButton : public UButton
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetCurIndex();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMyInvenUI* _invenUI;

	int32 _buttonIndex;
};
