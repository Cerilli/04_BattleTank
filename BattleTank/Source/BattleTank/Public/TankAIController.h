// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h" //generated.h must be last include

class UTankAimingComponent;

UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

protected:
	// How close the ai tank can get to the player
	UPROPERTY(EditDefaultsOnly, Category = "Setup") 
	float AcceptanceRadius = 8000.f;

private:
	virtual void BeginPlay() override;	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnPossessedTankDeath();
	
	// Gets called when controller is possesed 
	virtual void SetPawn(APawn* InPawn) override;

	APawn* ControlledTank = nullptr;
	UTankAimingComponent* AimingComponent = nullptr;
};
