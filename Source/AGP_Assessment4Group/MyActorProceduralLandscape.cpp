// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActorProceduralLandscape.h"

// Sets default values
AMyActorProceduralLandscape::AMyActorProceduralLandscape()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActorProceduralLandscape::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActorProceduralLandscape::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

