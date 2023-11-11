// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PointLightComponent.h"

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

    //  Variable to control conditions when Player enters or leaves the Player collision
    //  detection sphere
    UPROPERTY(Replicated)
    bool OverlapDetected = false;

    //  Variable to control conditions when the Line of Sight Debug Line Trace hit the player
    UPROPERTY(Replicated)
    bool EnemyHasLOS;

    //  Used to Handle the Transition from Rotating Behaviour to Patrolling Behaviour.
    FTimerHandle RotateTimerHandle;
    UPROPERTY(Replicated)
    int32 RotationCount = 0;

    //  Used to give the enemy Character "memory" of the Players last known location.
    //  Stores Player Location
    UPROPERTY(Replicated)
    FVector LastSeenLocation;
    UPROPERTY(Replicated)
    FVector DestinationLocation;

    //  Random Colour for Debug Lines and Spheres
    FColor RandomColor;

    //  Used by the Line of Sight DebugLineTrace
    UPROPERTY(Replicated)
    FVector StartLocation;

    //  Used by the Line of Sight DebugLineTrace
    UPROPERTY(Replicated)
    FVector EndLocation;

    //  Used by the Line of Sight DebugLineTrace to tell when an object
    //  is between the enemy and player
    UPROPERTY(Replicated)
    FHitResult HitResult;

    //  Used by the Line of Sight DebugLineTrace to tell when an object
    //  is between the enemy and player
    UPROPERTY(Replicated)
    bool bHit;

    // Light component
    UPROPERTY(Replicated)
    UPointLightComponent* PointLight;

    //Set up the Enemy AI Controller
    UPROPERTY(Replicated)
    class AEnemyAIController* EnemyAIController;

    //Reference to the Player
    UPROPERTY(EditAnywhere)
    class AAGP_Assessment4GroupCharacter* PlayerCharacter;

    //  Collision detection sphere used as the Enemy Character's "Sensing Sphere"
    //  This also sets the bounds for the Line of Sight Mechanic and used
    //  by the Overlap Begin and End functions
    UPROPERTY(EditAnywhere)
    class USphereComponent* PlayerCollisionDetection;

    //  Collision sphere used for damage range and trigger of blood splatter effect
    UPROPERTY(EditAnywhere)
    class USphereComponent* BloodSplatterCollision;

    //  Sets the min distance to the player to avoid mesh overlaps
    UPROPERTY(Replicated)
    float StoppingDistance = 2.0f;

    //  Used in RotateSearch function for timing between rotation phases
    UPROPERTY(Replicated)
    float PauseBetweenRotation = 2.0f;

    //  Set Visibility of Line Trace in Editor (DEPRECIATED - Will break functionality of line trace if set to false)
    UPROPERTY(Replicated)
    bool LineTraceVisible = true;

    //  This detects the overlap begin of the player character and the enemy sensing sphere.
    //  This function also is used as the bounds for the line of sight line trace.
    UFUNCTION(Client, Reliable)
    void OnPlayerDetectedOverlapBegin(class UPrimitiveComponent* OverlappedComp,                              
       class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
       int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    //  This function detects when the player has left the sensing sphere
    //  This is used to set the last seen location to the player characters location
    //  at the Overlap End point
    UFUNCTION(Client, Reliable)
    void OnPlayerDetectedOverlapEnd(class UPrimitiveComponent* OverlappedComp,                               
       class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
       int32 OtherBodyIndex);

    //  This function is used to trigger the blood splatter effect
    //  and the damage application to the player which is then used in blueprints to update
    //  the health bar widget. Sets the specific damage value per tick.
    UFUNCTION(NetMulticast, Reliable)
    virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

    //  This function is used to tell when the enemy character has completed a previously assigned action.
    //  This is used to know when to pass to RotateSearch()
    void OnAIMoveCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result);

    //  This is the functionality of the Search behaviour of the Enemy Character.
    //  It defines the characteristic of the rotating behaviour when the player
    //  character is not detected. It is also used to pass to the AI Controller
    //  where RandomPatrol() is called
    UFUNCTION(NetMulticast, Reliable)                                                                                  
    void RotateSearch();

    //  This is the Line of Sight Mechanic
    //  It is a debug line trace that only functions within the enemy sensing sphere and is used to tell
    //  if there is an object between the enemy and player. This is also used to set the last seen location
    //  of the player and actually command the enemy to chase the player or proceed to the last known location
    UFUNCTION(NetMulticast, Reliable)                                                                                
    void LineOfSightLineTrace();

    //  Sets a random colour that is used for all debug line traces and debug spheres
    UFUNCTION()                                                                                  
    static FColor GenerateRandomColor();

    //  Which variables are to be replicated (match those marked for replication in this header file)
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

    
protected:
    //  Niagara Particle system used for the blood splatter effect when the enemy and player
    //  characters collide and the player takes damage
    UPROPERTY(EditDefaultsOnly, Category="Particle System")
    UNiagaraSystem* PlayerHitParticles;

    
public:
    //  Call the Particle system
    UFUNCTION()
    void SpawnPlayerHitParticles(const FVector& SpawnLocation);

};
