// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MultiPlayerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AGP_ASSESSMENT4GROUP_API AMultiPlayerGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMultiPlayerGameMode();

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlayerController> PlayerController;


	
};
