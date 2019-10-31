// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

/**
 * 
 */

class ASprungWheel;

// TankTrack is used to set maximum driving force and apply forces to the tank
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()


public: 
	// Sets a throttle between -1 and +1 
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetThrottle(float Throttle);

protected:
	virtual void BeginPlay() override;

private:
	UTankTrack();
	void ApplySidewaysForce();
	void DriveTrack(float CurrentThrottle);

	TArray<ASprungWheel*> GetWheels() const;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float TrackMaxDrivingForce = 40000000.0; // Assume 40 tonne tank and 1g acceleration
		
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
