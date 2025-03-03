// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"

#include "MyCharacter.h"

AMyGameModeBase::AMyGameModeBase()
{
	static ConstructorHelpers::FClassFinder<AMyCharacter>
		pawn(TEXT("/Script/Engine.Blueprint'/Game/BluePrints/BP_MyPlayer.BP_MyPlayer_C'"));

	if (pawn.Succeeded())
	{
		DefaultPawnClass = pawn.Class;
	}
}
