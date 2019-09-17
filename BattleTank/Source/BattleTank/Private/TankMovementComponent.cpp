// Fill out your copyright notice in the Description page of Project Settings.


#include "TankMovementComponent.h"
#include "TankTrack.h"


void UTankMovementComponent::Initialise(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{	
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;	
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!LeftTrack || !RightTrack) { return; }
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	if (!LeftTrack || !RightTrack) { return; }
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	// No need to call super, as we're replacing the functionality here

	auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto AIForwardInention = MoveVelocity.GetSafeNormal();	
	
	auto RightDirection = FVector::CrossProduct(TankForward, AIForwardInention).Z;
	auto ForwardDirection = FVector::DotProduct(TankForward, AIForwardInention);	

	IntendTurnRight(RightDirection);
	IntendMoveForward(ForwardDirection);

	//UE_LOG(LogTemp, Warning, TEXT("Dot Product: %f"), ForwardDirection)
	//UE_LOG(LogTemp, Warning, TEXT("Cross Product: %f"), RightDirection)
}
