// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "TankAIController.h"
// Depends on movement component via pathfinding system

void ATankAIController::BeginPlay()
{

	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();	
	auto ControlledTank = GetPawn();

	if (!ensure(PlayerTank && ControlledTank)) { return; }
	// Move towards the player
	MoveToActor(PlayerTank, AcceptanceRadius);
		
	// Aim towards the player
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	// Fire if ready
	/*
	if (ControlledTank->bCanFire)
	{
		ControlledTank->Fire();
	}
	*/
}