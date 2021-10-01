// Copyright Epic Games, Inc. All Rights Reserved.

#include "AtomaGameMode.h"
#include "AtomaPlayerController.h"
#include "AtomaCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAtomaGameMode::AAtomaGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AAtomaPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}