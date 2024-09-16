// Copyright Epic Games, Inc. All Rights Reserved.

#include "LonelySwordGameMode.h"
#include "LonelySwordCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALonelySwordGameMode::ALonelySwordGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
