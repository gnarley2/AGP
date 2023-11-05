// Fill out your copyright notice in the Description page of Project Settings.

// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"

#include "EnemyCharacter.h"


AMultiPlayerGameMode::AMultiPlayerGameMode()
{
	// Set the default pawn class for this game mode.
	DefaultPawnClass = AEnemyCharacter::StaticClass(); // Use AEnemyCharacter as the default pawn class
}
