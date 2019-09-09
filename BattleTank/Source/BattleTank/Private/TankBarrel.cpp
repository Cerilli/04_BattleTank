// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrel.h"

void UTankBarrel::Elevate(float RelativeSpeed)
{
	// Move barrel the right amount this frame
	// Given a max elevation speed, and the frame time

	// The elevation channge we need for this frame	
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);	
	auto ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	
	// The rotation of the barrel without any constraints
	auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;
	
	// Clamp to min and max rotation
	auto NewElevation = FMath::Clamp<float>(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);	
	
	// Rotate the barrel
	SetRelativeRotation(FRotator(NewElevation, 0, 0));

}