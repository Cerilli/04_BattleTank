// Stinner

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"  //generated.h must be last include


// Forward declaration
class UTankAimingComponent;

/**
 * Tank controller stuff
 */

UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// override BeginPlay() from parent class. In this case it's AActor
	void BeginPlay() override;

	virtual void Tick(float) override;

	// Start the tank moving the barrel so that a shot would hit
	// where the crosshair intersects the world
	void AimTowardsCrosshair();

protected:
	// Create an event that passes out the Tank Aiming component	
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* AimingCompRef);

private:
	// Return an OUT parameter, true if hit landscape
	// Out parameters work by passing references to
	bool GetSightRayHitLocation(FVector& OutHitLocation) const;
	bool GetLookVectorHitLocation(FVector LookDirection, FVector & OutHitLocation) const;
	bool GetLookDirection(FVector2D ScreenLocation, FVector & LookDirection) const;
	
	UPROPERTY(EditDefaultsOnly)
	float CrossHairXLocation = 0.5;

	UPROPERTY(EditDefaultsOnly)
	float CrossHairYLocation = 0.3;

	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 10000000; //cm's, so it's 10km	

	UTankAimingComponent* AimingComponent = nullptr;
};
