// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"

#include "EnemyCharacter.h"
#include "NavigationSystem.h"



void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	FVector RandomLocation;
	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), GetPawn()->GetActorLocation(),
	RandomLocation, 1000.0f);
	//RandomPatrol();

}


void AEnemyAIController::RandomPatrol()
{
	if (NavArea)
	{
		FVector RandomLocation;
		int32 RandomRadius = FMath::RandRange(750, 1200);
		UE_LOG(LogTemp, Warning, TEXT("Player is Lost; Enemy searching a new location"));
		NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), GetPawn()->GetActorLocation(),
		RandomLocation, RandomRadius);
		MoveToLocation(RandomLocation);
	}
}
