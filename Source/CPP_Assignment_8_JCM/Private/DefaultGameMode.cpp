// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameMode.h"
#include "SpartaGameState.h"

ADefaultGameMode::ADefaultGameMode()
{
	GameStateClass = ASpartaGameState::StaticClass();
}
