// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "EnemyAIController.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"


UCLASS()
class AGP_ASSESSMENT4GROUP_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
 
    // Sets default values for this character's properties
    AEnemyCharacter();

protected:

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    UPROPERTY(Replicated)
    bool PlayerDetected;
    UPROPERTY(Replicated)
    bool OverlapDetected = false;
    UPROPERTY(Replicated)
    bool EnemyHasLOS;
    
    FTimerHandle RotateTimerHandle;
    UPROPERTY(Replicated)
    int32 RotationCount = 0;
    UPROPERTY(Replicated)
    FVector LastSeenLocation;
    UPROPERTY(Replicated)
    FVector DestinationLocation;
    FColor RandomColor;
    UPROPERTY(Replicated)
    FVector StartLocation;
    UPROPERTY(Replicated)
    FVector EndLocation;
    UPROPERTY(Replicated)
    FHitResult HitResult;
    UPROPERTY(Replicated)
    bool bHit;
	
    UPROPERTY(Replicated)
    class AEnemyAIController* EnemyAIController;
    
    UPROPERTY(EditAnywhere)
    class AAGP_Assessment4GroupCharacter* PlayerCharacter;

    //UPROPERTY(Replicated)
    //class AMyProject3Character* PlayerCharacter2;
    
    UPROPERTY(EditAnywhere)
    class USphereComponent* PlayerCollisionDetection;

    UPROPERTY(EditAnywhere)
    class USphereComponent* BloodSplatterCollision;
    
    UPROPERTY(Replicated)
    float StoppingDistance = 2.0f;
    
    UPROPERTY(Replicated)
    float PauseBetweenRotation = 2.0f;

    UPROPERTY(Replicated)
    bool LineTraceVisible = true;
    
    UFUNCTION(Client, Reliable)
    void OnPlayerDetectedOverlapBegin(class UPrimitiveComponent* OverlappedComp,                              //Function detects when overlap of Sphere and PlayerCharacter Begins
       class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
       int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(Client, Reliable)
    void OnPlayerDetectedOverlapEnd(class UPrimitiveComponent* OverlappedComp,                               //Function detects when overlap of Sphere and PlayerCharacter Ends
       class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
       int32 OtherBodyIndex);

    UFUNCTION(NetMulticast, Reliable)
    virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

    void OnAIMoveCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result);
    
    UFUNCTION(Client, Reliable)                                                                                  //Function to control rotating behaviour when PlayerCharacter is not Sensed
    void RotateSearch();
    
    UFUNCTION(NetMulticast, Reliable)                                                                                //Function for Line of Sight Mechanic; Tells the EnemyCharacter whether to chase Player or Search for Player
    void LineOfSightLineTrace();
    
    UFUNCTION()                                                                                  //Generate a different colour for debug for each enemy 
    static FColor GenerateRandomColor();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

protected:
    UPROPERTY(EditDefaultsOnly, Category="Particle System")
    UNiagaraSystem* PlayerHitParticles;

public:
    UFUNCTION()
    void SpawnPlayerHitParticles(const FVector& SpawnLocation);
};
