// Fill out your copyright notice in the Description page of Project Settings.

// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"

#include "EnemyCharacter.h"


void AMultiPlayerGameMode::AMultiplayerGameMode()
{
	// Set the default pawn class for this game mode.
	DefaultPawnClass = AAGP_Assessment4GroupCharacter::StaticClass(); // Use AEnemyCharacter as the default pawn class
}
