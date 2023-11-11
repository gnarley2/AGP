// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "EnemyAIController.h"
#include "AGP_Assessment4GroupCharacter.h"
#include "Components/SphereComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlayerCollisionDetection = CreateDefaultSubobject<USphereComponent>(TEXT("Player Collision Detection"));
	PlayerCollisionDetection->SetupAttachment(RootComponent);

	
	BloodSplatterCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Blood Splatter Collision"));
	BloodSplatterCollision->SetupAttachment(RootComponent);


	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(RootComponent);
	PointLight->Intensity = 1000.0f;
	
}


// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Play has Begun"));
	PlayerDetected = false;
	
	PlayerCharacter = Cast<AAGP_Assessment4GroupCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	EnemyAIController = Cast<AEnemyAIController>(GetController());
	
	PlayerCollisionDetection->OnComponentBeginOverlap.AddDynamic(this,
	&AEnemyCharacter::OnPlayerDetectedOverlapBegin);
	
	PlayerCollisionDetection->OnComponentEndOverlap.AddDynamic(this,
	&AEnemyCharacter::OnPlayerDetectedOverlapEnd);

	if (EnemyAIController)
	{
		EnemyAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AEnemyCharacter::OnAIMoveCompleted);
	}
	
	RandomColor = GenerateRandomColor();
	
}


// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
if(HasAuthority())
{
	if(LineTraceVisible)																									//If set in the editor
		{
		LineOfSightLineTrace();																								//perform line trace. Set up to only trace when player is within Enemy Collision Sphere. This is for line of sight mechanic.
		}
}
}

void AEnemyCharacter::OnPlayerDetectedOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		AAGP_Assessment4GroupCharacter* OverlappingPlayer = Cast<AAGP_Assessment4GroupCharacter>(OtherActor);
		if (OverlappingPlayer)
		{
			PlayerCharacter = OverlappingPlayer;
			OverlapDetected = true;
		}
	}
	else
	{
		AAGP_Assessment4GroupCharacter* OverlappingPlayer = Cast<AAGP_Assessment4GroupCharacter>(OtherActor);
		if (OverlappingPlayer)
		{
			PlayerCharacter = OverlappingPlayer;
			OverlapDetected = true;
		}
	}

}

void AEnemyCharacter::OnPlayerDetectedOverlapEnd_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(HasAuthority())
	{
		if (OtherActor->IsA(AAGP_Assessment4GroupCharacter::StaticClass()))
		{
			{
				EnemyHasLOS=false;
				OverlapDetected = false;
				LastSeenLocation.Z -= 90;
				DrawDebugSphere(GetWorld(),  LastSeenLocation, 10.0f, 8, RandomColor, false, 2.0f);
			}
		}
	}
	else
	{
		if (OtherActor->IsA(AAGP_Assessment4GroupCharacter::StaticClass()))
		{
			{
				EnemyHasLOS=false;
				OverlapDetected = false;
				LastSeenLocation.Z -= 90;
				DrawDebugSphere(GetWorld(),  LastSeenLocation, 10.0f, 8, RandomColor, false, 2.0f);
			}
		}
	}
	}

//Blood Splatter on Hit with character and hit radius
void AEnemyCharacter::NotifyHit_Implementation(UPrimitiveComponent* MyComp, AActor* Other,
	UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
	const FHitResult& Hit)
{
	if(HasAuthority())
	{
		if (Other->IsA(AAGP_Assessment4GroupCharacter::StaticClass()))
		{
			SpawnPlayerHitParticles(EndLocation);
			PlayerCharacter->ApplyDamage(0.5);
		}
	}
	else
	{
		if (Other->IsA(AAGP_Assessment4GroupCharacter::StaticClass()))
		{
			SpawnPlayerHitParticles(EndLocation);
			PlayerCharacter->ApplyDamage(0.5);
		}
	}
	
}

void AEnemyCharacter::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	const FVector CurrentVelocity = GetMovementComponent()->Velocity;
	const float CurrentSpeed = CurrentVelocity.Size();

		if(CurrentSpeed==0.0f&&!EnemyHasLOS	)															//Used to tell when to call RotateSearch() from moving
		{
			RotateSearch();
		}

}


UENUM(BlueprintType)																										//this is used to keep track of states and allow the Enemy to rotate with no line of sight but with Overlap Detected
enum class ERotationState : uint8
{
	Idle,
	Rotating,
};

ERotationState RotationState = ERotationState::Idle;


void AEnemyCharacter::RotateSearch_Implementation()																						//This function handles the rotation interpolation and either calls itself again or passes to RandomPatrol()
{
	PointLight->SetLightColor(FLinearColor(0.0f, 1.0f, 0.0f));
	GetCharacterMovement()->MaxWalkSpeed =300;
	
if(HasAuthority())
{
	PointLight->SetLightColor(FLinearColor(0.0f, 1.0f, 0.0f));
	if (RotationState == ERotationState::Idle)
	{
		const FVector CurrentVelocity = GetMovementComponent()->Velocity;
		const float CurrentSpeed = CurrentVelocity.Size();
		float RandomRotation = FMath::FRandRange(-180.0f, 180.0f);

		if (CurrentSpeed == 0.0f && !EnemyHasLOS)																			//Extra conditions and checks in this function serve to prevent rotating when moving
			{

			RotationState = ERotationState::Rotating;
			const FRotator RotationRate(0.f, RandomRotation, 0.f);
			const FRotator TargetRotation = GetActorRotation() + RotationRate;
			const float UpdateRate = 0.01f;
			const int32 NumSteps = FMath::CeilToInt(PauseBetweenRotation / UpdateRate);									//time between rotations
			FTimerDelegate TimerCallback;
			TimerCallback.BindLambda([=]()
			{
				static int32 CurrentStep = 0;
				if (CurrentStep < NumSteps && RotationState == ERotationState::Rotating && !EnemyHasLOS&&CurrentSpeed==0.0f&&RotationCount<2)					//This part controls the rotation and the interpolation between the positions
					{
						const FRotator NewRotation = FMath::Lerp(GetActorRotation(), TargetRotation, static_cast<float>(CurrentStep) / static_cast<float>(NumSteps));
						SetActorRotation(NewRotation);
						CurrentStep++;
			
					}
				
					else
					{
						RotationState = ERotationState::Idle;
						CurrentStep = 0;
						GetWorldTimerManager().ClearTimer(RotateTimerHandle);
					
						if (RotationCount >= 2 )
						{

							RotationCount = 0;
							EnemyAIController->RandomPatrol();
						}
						else if(RotationCount <=1 &&!EnemyHasLOS)
						{
							RotateSearch();
							RotationCount++;

						}

					}
			
				});
			GetWorldTimerManager().ClearTimer(RotateTimerHandle);
			GetWorldTimerManager().SetTimer(RotateTimerHandle, TimerCallback, UpdateRate, true, 0.1f);
			}
	}
}
else
{
	PointLight->SetLightColor(FLinearColor(0.0f, 1.0f, 0.0f));
}
}

void AEnemyCharacter::LineOfSightLineTrace_Implementation()																				//For line of Sight Mechanics
{
	if(HasAuthority())
	{
		StartLocation = GetActorLocation();
		EndLocation = PlayerCharacter->GetActorLocation();

		bHit = GetWorld()->LineTraceSingleByChannel(HitResult,StartLocation,EndLocation,ECC_Visibility );

		if (bHit&&OverlapDetected)																								// The line trace hit something before reaching the player
			{
			EnemyHasLOS = false;
			EnemyAIController->MoveToLocation( LastSeenLocation, StoppingDistance, false);

			}
		if(!bHit&&OverlapDetected)																								// There's a clear line of sight to the player within the sphere
			{
			RotationCount = 0;
			EnemyHasLOS=true;
			LastSeenLocation = PlayerCharacter->GetActorLocation();
			PointLight->SetLightColor(FLinearColor(1.0f, 0.0f, 0.0f));
			GetCharacterMovement()->MaxWalkSpeed =400;
			EnemyAIController->MoveToLocation( LastSeenLocation, StoppingDistance, false);

			DrawDebugLine(GetWorld(),StartLocation,EndLocation,RandomColor,false,0.20,0,0.25);
			const float Distance = FVector::Distance(StartLocation, EndLocation);

			if(Distance<150.0f&&Distance>140.0f)																				//Threshold for distance to player for enemy to jump
				{
				const float RandomZVelocity = FMath::FRandRange(150.0, 450.0);										//Jump height randomizer, to help feel more varied
				GetCharacterMovement()->JumpZVelocity = RandomZVelocity;
				Jump();
				}
			}
	}
	else
	{
		bHit = GetWorld()->LineTraceSingleByChannel(HitResult,StartLocation,EndLocation,ECC_Visibility );
	
		if (bHit&&OverlapDetected)																								// The line trace hit something before reaching the player
			{
			}
		if(!bHit&&OverlapDetected)																								// There's a clear line of sight to the player within the sphere
			{
			DrawDebugLine(GetWorld(),StartLocation,EndLocation,RandomColor,false,0.20,0,0.25);
			PointLight->SetLightColor(FLinearColor(1.0f, 0.0f, 0.0f));
			}
	}
}

 FColor AEnemyCharacter::GenerateRandomColor()																				//Random Colour for each EnemyCharacter's Debug line and matching spheres
{
	const FRandomStream RandomStream(FMath::Rand());

	// Generate random values for each RGBA component
	const uint8 RandomR = RandomStream.RandRange(0, 255);
	const uint8 RandomG = RandomStream.RandRange(0, 255);
	const uint8 RandomB = RandomStream.RandRange(0, 255);
	constexpr uint8 RandomA = 200; // Alpha component can be fully opaque (255) or adjusted as needed

	// Create and return the random color
	const FColor RandomColorX(RandomR, RandomG, RandomB, RandomA);
	return RandomColorX;
}
void AEnemyCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEnemyCharacter, PlayerDetected);
	DOREPLIFETIME(AEnemyCharacter, EnemyHasLOS);
	DOREPLIFETIME(AEnemyCharacter, LastSeenLocation);
	DOREPLIFETIME(AEnemyCharacter, DestinationLocation);
	DOREPLIFETIME(AEnemyCharacter, StoppingDistance);
	DOREPLIFETIME(AEnemyCharacter, PauseBetweenRotation);
	DOREPLIFETIME(AEnemyCharacter, LineTraceVisible);
	DOREPLIFETIME(AEnemyCharacter, StartLocation);
	DOREPLIFETIME(AEnemyCharacter, EndLocation);
	DOREPLIFETIME(AEnemyCharacter, HitResult);
	DOREPLIFETIME(AEnemyCharacter, bHit);
	DOREPLIFETIME(AEnemyCharacter, RotationCount);
	DOREPLIFETIME(AEnemyCharacter, OverlapDetected);
	DOREPLIFETIME(AEnemyCharacter, EnemyAIController);
	DOREPLIFETIME(AEnemyCharacter, PointLight);
	
	// Add other variables you want to replicate here.
}

void AEnemyCharacter::SpawnPlayerHitParticles(const FVector& SpawnLocation)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PlayerHitParticles, SpawnLocation);
}
//The Enemy Character can "See" and "Hear" the PlayerCharacter with Line of Sight implemented.
//
//The Sphere is the Range of the Enemy's Senses.
//If Player is within the sphere, the line trace occurs.
//If line trace not blocked by anything, enemy moves towards the player. This is the Line of Sight mechanic and how the Enemy can "See" the Player.
//if line trace is blocked or player leaves the Sphere, the Enemy moves to The Players location when line trace or Sphere collision stopped(last known location).
//When the Enemy reaches the last known location, it performs a series of rotations "scanning" for the player.
//If no sphere collision occurs, the Enemy proceeds to move to another location "searching" for the player. It will then repeat the scanning and searching behaviours.
//If a sphere collision does occur while scanning, the Enemy will continue scanning until no sphere collision is detected but does not chase. This is the enemy "Hearing" or "Sensing" the Player.
//

