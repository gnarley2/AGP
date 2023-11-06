// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AGP_Assessment4GroupCharacter.h"
#include "EnemyAIController.generated.h"


/**
 * 
 */
UCLASS()
class AGP_ASSESSMENT4GROUP_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()


public:
	virtual void BeginPlay() override;
    
	void RandomPatrol();
    
	FAIRequestID RequestID;

	bool RandomPatrolComplete = false;

	//UPROPERTY()
	//class AEnemyAIController* EnemyAIController;
	UPROPERTY()
	class AAGP_Assessment4GroupCharacter* PlayerCharacter;
       
	UPROPERTY(EditAnywhere)
	float StoppingDistance = 10.0f;
	FTimerHandle InterpTimerHandle;
	bool bIsPaused;
	float ResumeTimeAfterPause;
	FTimerHandle DelayHandle;
	FTimerHandle PauseTimerHandle;
	FTimerHandle RotateTimerHandle;
	UPROPERTY(EditAnywhere)
	class AEnemyCharacter* EnemyCharacter;
	
	UPROPERTY(EditAnywhere)
	float PauseBetweenRotation = 2.0f;
	int32 RotationCount = 0;
	bool EnableRoam = true;
private:

	UPROPERTY(EditAnywhere)
	class UNavigationSystemV1* NavArea;   ///get random points that you can travel too


	
};
