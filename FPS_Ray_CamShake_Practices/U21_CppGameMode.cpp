// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "U21_CppGameMode.h"
#include "U21_CppHUD.h"
#include "U21_CppCharacter.h"
#include "UObject/ConstructorHelpers.h"

AU21_CppGameMode::AU21_CppGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AU21_CppHUD::StaticClass();
}
