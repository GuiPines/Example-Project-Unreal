// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Core/ExampleProjectCharacter.h"
#include "Core/CoinsGameStateBase.h"
#include <Kismet/GameplayStatics.h>
#include <Net/UnrealNetwork.h>
#include <Core/ExampleProjectGameMode.h>

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create AI Perception Component
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// Create and configure Sight Sense for AI perception
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig")); // Create the sight sense config object

	SightConfig->SightRadius = 1500.0f; // Maximum distance at which the AI can see targets
	SightConfig->LoseSightRadius = 1800.0f; // Distance at which the AI loses sight of targets (hysteresis)
	SightConfig->PeripheralVisionAngleDegrees = 90.0f; // Field of view angle (degrees) for peripheral vision
	SightConfig->SetMaxAge(5.0f); // How long a stimulus is valid after being sensed (in seconds)
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f; // Range within which AI will automatically succeed in sensing if target was recently seen
	
	// Configure which affiliations the AI can detect
	SightConfig->DetectionByAffiliation.bDetectEnemies = true; // Detect enemies
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; // Detect neutral actors
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true; // Detect friendly actors
	
	// Add sight configuration to perception component
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	this->GetMesh()->GlobalAnimRateScale = 1.0f;
	this->GetCharacterMovement()->MaxWalkSpeed = 100;

	// Bind perception update event
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyCharacter::OnTargetPerceptionUpdated);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay() {

	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), Waypoints);
	AIController = Cast<AAIController>(GetController());
	if ((Waypoints.Num() > 0) && (AIController))
	{
		AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this,
			&AEnemyCharacter::AIMoveCompleted);
		if (HasAuthority())
			AIController->MoveToActor(GetRandomWaypoint());
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

ATargetPoint* AEnemyCharacter::GetRandomWaypoint() {
	int index = FMath::RandRange(0, Waypoints.Num() - 1);
	return Cast<ATargetPoint>(Waypoints[index]);
}

//void AEnemyCharacter::AIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) {
//	if (Result.IsSuccess() && HasAuthority())
//	{
//		if (target) {
//			AExampleProjectCharacter* character = Cast<AExampleProjectCharacter>(target);
//			ACoinsGameStateBase* gamestate = Cast<ACoinsGameStateBase>(GetWorld()->GetGameState());
//
//			if ((character) && (gamestate)) {
//				gamestate->MulticastOnLevelComplete(character, false);
//			}
//
//			target = nullptr;
//		}
//
//		if ((Waypoints.Num() > 0) && (AIController)) {
//			AIController->MoveToActor(GetRandomWaypoint());
//		}
//	}
//}

void AEnemyCharacter::AIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess() && HasAuthority())
	{
		if (target)
		{
			AExampleProjectCharacter* character = Cast<AExampleProjectCharacter>(target);
			ACoinsGameStateBase* gamestate = Cast<ACoinsGameStateBase>(GetWorld()->GetGameState());

			if (character)
			{
				UE_LOG(LogTemp, Warning, TEXT("enemy %s caught player %s"), *GetName(), *character->GetName());

					if (AExampleProjectGameMode* GameMode = Cast<AExampleProjectGameMode>(GetWorld()->GetAuthGameMode()))
					{
						if (AController * PlayerController = character->GetController())
						{
							GameMode->RespawnPlayer(PlayerController);
							UE_LOG(LogTemp, Warning, TEXT("trigger respawn player %s"), *character->GetName());
						}
					}
			}

			target = nullptr;


		}

		if ((Waypoints.Num() > 0) && (AIController))
		{
			AIController->MoveToActor(GetRandomWaypoint());
		}
	}
}

//void AEnemyCharacter::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
//{
//	if (!HasAuthority()) return;
//
//	// Check if the stimulus was successfully sensed (seen)
//	if (Stimulus.WasSuccessfullySensed())
//	{
//		// Actor was detected
//		APawn* DetectedPawn = Cast<APawn>(Actor);
//
//		if (DetectedPawn && AIController && !target)
//		{
//			target = DetectedPawn;
//			this->GetMesh()->GlobalAnimRateScale = 2.5f;
//			this->GetCharacterMovement()->MaxWalkSpeed = 150.0f;
//			AIController->MoveToActor(DetectedPawn);
//			UE_LOG(LogTemp, Log, TEXT("Enemy detected player: %s"), *Actor->GetName());
//		}
//	}
//	else
//	{
//		// Actor was lost from sight
//		if (Actor == target)
//		{
//			this->GetMesh()->GlobalAnimRateScale = 1.0f;
//			this->GetCharacterMovement()->MaxWalkSpeed = 100;
//			target = nullptr;
//			AIController->MoveToActor(GetRandomWaypoint());
//			UE_LOG(LogTemp, Log, TEXT("Enemy target out of range, returning to patrol"));
//		}
//	}
//}

void AEnemyCharacter::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!HasAuthority()) return;
	// Check if the stimulus was successfully sensed (seen)
	if (Stimulus.WasSuccessfullySensed())
	{
		// Actor was detected
		APawn* DetectedPawn = Cast<APawn>(Actor);
		if (DetectedPawn && AIController && !target)
		{
			target = DetectedPawn;
			SetEnemySpeed(150.0f, 2.5f);
			AIController->MoveToActor(DetectedPawn);
			UE_LOG(LogTemp, Log, TEXT("Enemy detected player: %s"), *Actor->GetName());
		}
	}
	else
	{
		// Actor was lost from sight
		if (Actor == target)
		{
			SetEnemySpeed(100.0f, 1.0f);
			target = nullptr;
			AIController->MoveToActor(GetRandomWaypoint());
			UE_LOG(LogTemp, Log, TEXT("Enemy target out of range, returning to patrol"));
		}
	}
}

void AEnemyCharacter::SetEnemySpeed(float NewSpeed, float NewAnimRate)
{
	if (HasAuthority())
	{
		CurrentSpeed = NewSpeed;
		AnimationRate = NewAnimRate;

		// Update locally on server
		OnRep_CurrentSpeed();
		OnRep_AnimationRate();
	}
}

// Replication functions
void AEnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEnemyCharacter, CurrentSpeed);
	DOREPLIFETIME(AEnemyCharacter, AnimationRate);
}

void AEnemyCharacter::OnRep_CurrentSpeed()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
		UE_LOG(LogTemp, Log, TEXT("Enemy speed updated to: %f"), CurrentSpeed);
	}
}

void AEnemyCharacter::OnRep_AnimationRate()
{
	if (GetMesh())
	{
		GetMesh()->GlobalAnimRateScale = AnimationRate;
		UE_LOG(LogTemp, Log, TEXT("Enemy animation rate updated to: %f"), AnimationRate);
	}
}