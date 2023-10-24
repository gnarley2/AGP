// Copyright Epic Games, Inc. All Rights Reserved.

#include "AGP_Assessment4GroupGameMode.h"
#include "AGP_Assessment4GroupCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAGP_Assessment4GroupGameMode::AAGP_Assessment4GroupGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
