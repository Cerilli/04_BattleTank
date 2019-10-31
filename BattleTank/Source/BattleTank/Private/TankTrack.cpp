// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"
#include "Components\PrimitiveComponent.h"
#include "SprungWheel.h"
#include "SpawnPoint.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay()
{
	//Super::BeginPlay();

	// DEPRECATED - by spring wheel technique
	// OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);  
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// No longer wanted on hit after spring wheel technique. Used in SetThrottle instead
	// DriveTrack(); 
	// ApplySidewaysForce();
	// CurrentThrottle = 0;  --No longer a member variable
}

// DEPRECATED - This method is completely deprecated by using the Spring Wheel technique instead
void UTankTrack::ApplySidewaysForce()
{
	// Calculate the sideways slippage speed (using dot product)
	auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());

	// Work out required correction acceleration
	auto DeltaTime = GetWorld()->GetDeltaSeconds();
	auto CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();

	// Calculate and apply sideways force (f = ma)
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	auto CorrectionForce = TankRoot->GetMass() * CorrectionAcceleration / 2; //Two tracks
	TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::SetThrottle(float Throttle)
{
	float CurrentThrottle = FMath::Clamp<float>(Throttle, -1, 1);
	//UE_LOG(LogTemp, Warning, TEXT("CurrentThrottle: %f"), CurrentThrottle)
	DriveTrack(CurrentThrottle);
}

void UTankTrack::DriveTrack(float CurrentThrottle)
{

	// Pre spring wheel technique DEPRECATED -------------------
	// TODO clamp actual throttle value
	// auto ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
	// auto ForceLocation = GetComponentLocation() + FVector(0.f, 0.f, 60.f);
	// auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	// TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
	// --------------------------------------------

	// Determine the force required per wheel and apply it to each wheel found
	auto ForceApplied = CurrentThrottle * TrackMaxDrivingForce;
	auto Wheels = GetWheels();
	auto ForcePerWheel = ForceApplied / Wheels.Num();

	for (ASprungWheel* Wheel : Wheels)
	{
		Wheel->AddDrivingForce(ForcePerWheel);
	}
}

TArray<ASprungWheel*> UTankTrack::GetWheels() const
{
	TArray<ASprungWheel*> ResultArray;
	TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);	

	for (USceneComponent* Child : Children)
	{		
		auto SpawnPointChild = Cast<USpawnPoint>(Child);
		if (!SpawnPointChild) continue;

		AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor();
		auto SprungWheel = Cast<ASprungWheel>(SpawnedChild);
		if (!SprungWheel) continue;		

		ResultArray.Add(SprungWheel);
	}
	return ResultArray;
}