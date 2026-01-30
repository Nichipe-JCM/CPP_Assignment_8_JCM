// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameModeBase.h"
#include "SpartaGameStateBase.h"

ADefaultGameModeBase::ADefaultGameModeBase()
{
	GameStateClass = ASpartaGameStateBase::StaticClass();
}
