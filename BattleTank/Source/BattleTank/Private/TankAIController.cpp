// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "TankAIController.h"
#include "Tank.h" // So we can implement OnDeath
// Depends on movement component via pathfinding system

void ATankAIController::BeginPlay()
{

	Super::BeginPlay();
	ControlledTank = GetPawn();
	AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		// Subscribe our local method to the tank's death event
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();	

	if (!ensure(PlayerTank && ControlledTank)) { return; }
	
	// Move towards the player
	UE_LOG(LogTemp, Warning, TEXT("Player Tank: %s"), *PlayerTank->GetName())
	MoveToActor(PlayerTank, AcceptanceRadius);
		
	// Aim towards the player
	if (!ensure(AimingComponent)) { return; }
	AimingComponent->AimAt(PlayerTank->GetActorLocation());
	// Fire if ready
	if (AimingComponent->bCanFire)
	{
		if (AimingComponent->GetFiringState() == EFiringState::LOCKED)
		{
			AimingComponent->Fire();
		}
	}
}