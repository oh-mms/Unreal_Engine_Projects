// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "U14_CppGameMode.h"
#include "U14_CppHUD.h"
#include "U14_CppCharacter.h"
#include "UObject/ConstructorHelpers.h"

AU14_CppGameMode::AU14_CppGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AU14_CppHUD::StaticClass();
}
