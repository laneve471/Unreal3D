// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInvenButton.h"

#include "MyInvenUI.h"
#include "MyInvenComponent.h"
#include "MyToolTip.h"

void UMyInvenButton::SetCurIndex()
{
	if (_invenUI == nullptr)
		return;

	_invenUI->_curIndex = _buttonIndex;
}

void UMyInvenButton::ShowToolTip()
{
	SetToolTip(_toolTip);
}