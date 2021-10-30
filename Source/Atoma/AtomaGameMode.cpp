// Copyright Epic Games, Inc. All Rights Reserved.

#include "AtomaGameMode.h"
#include "AtomaCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAtomaGameMode::AAtomaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
