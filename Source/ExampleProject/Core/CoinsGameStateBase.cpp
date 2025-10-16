// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinsGameStateBase.h"
#include <Kismet/GameplayStatics.h>
#include <Net/UnrealNetwork.h>
#include "../Collectibles/CoinActor.h"
#include "../Gameplay/PressurePlate.h"


ACoinsGameStateBase::ACoinsGameStateBase()
{
	// Enable tick for puzzle state checking
	PrimaryActorTick.bCanEverTick = true;

	// Initialize puzzle state
	bPuzzleCompleted = false;
	PressurePlates.Empty();

	UpdateTotalCoinsInLevel();
}

void ACoinsGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	// Find all pressure plates in the level
	FindPressurePlates();
}

void ACoinsGameStateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check puzzle state every frame on server
	if (HasAuthority())
	{
		CheckPuzzleState();
	}
}

void ACoinsGameStateBase::UpdateTotalCoinsInLevel()
{
	TArray<AActor*> FoundCoins;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoinActor::StaticClass(), FoundCoins);
	TotalLevelCoins = FoundCoins.Num();
	UE_LOG(LogTemp, Warning, TEXT("Update Total Coins"));
}

void ACoinsGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACoinsGameStateBase, TotalLevelCoins);
	DOREPLIFETIME(ACoinsGameStateBase, bPuzzleCompleted);
}

void ACoinsGameStateBase::MulticastOnLevelComplete_Implementation(APawn* character, bool succeeded)
{
	OnLevelCompleted(character, succeeded);
}

void ACoinsGameStateBase::FindPressurePlates()
{
	// Automatically find all pressure plates in the level
	TArray<AActor*> FoundPlates;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APressurePlate::StaticClass(), FoundPlates);

	PressurePlates.Empty();
	for (AActor* Plate : FoundPlates)
	{
		if (APressurePlate* PressurePlate = Cast<APressurePlate>(Plate))
		{
			PressurePlates.Add(PressurePlate);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("GameState found %d pressure plates for puzzle."), PressurePlates.Num());
}

void ACoinsGameStateBase::OnRep_PuzzleCompleted()
{
	// Called on clients when puzzle completion state changes
	
	// Here you could trigger visual/audio feedback for puzzle completion
}

void ACoinsGameStateBase::CheckPuzzleState()
{
	// Only run on server
	if (!HasAuthority())
		return;

	// Check if all pressure plates are activated
	bool bAllPlatesActive = true; // Start with true, then check if any are inactive
	if (PressurePlates.Num() > 0)
	{
		for (APressurePlate* Plate : PressurePlates)
		{
			if (Plate && !Plate->IsActivated())
			{
				bAllPlatesActive = false;
				break;
			}
		}
	}
	else
	{
		bAllPlatesActive = true;
	}

	// Check if all coins are collected
	bool bAllCoinsCollected = (TotalLevelCoins == 0);

	// Update puzzle completion state
	bool bNewPuzzleState = bAllPlatesActive && bAllCoinsCollected;
	if (bNewPuzzleState != bPuzzleCompleted)
	{
		bPuzzleCompleted = bNewPuzzleState;

		// Call OnRep manually on server since it only auto-calls on clients
		OnRep_PuzzleCompleted();
	}
}

