// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "BattleTank.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "Projectile.h"
#include "TankTurret.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (RoundsLeft <= 0)
	{
		FiringState = EFiringState::OUTOFAMMO;
	}
	
	if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
	{
		FiringState = ::EFiringState::RELOADING;
	}
	else if (IsBarrelMoving())
	{
		FiringState = EFiringState::AIMING;
	}
	else
	{
		FiringState = EFiringState::LOCKED;
	}
}

int UTankAimingComponent::GetRoundsLeft() const
{
	return RoundsLeft;
}

void UTankAimingComponent::BeginPlay()
{
	// So that first fire is after initial reload
	LastFireTime = FPlatformTime::Seconds();
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

void UTankAimingComponent::Initialise(UTankBarrel * BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel) || !ensure(Turret)) { return; } // protect pointer, always

	FVector OutLaunchVelocity;
	
	// Returns location of socket, if socket not found, returns location of Barrel
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));  // find socket on the fbx mesh

	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
	(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::OnlyTraceWhileAscending,
		FCollisionResponseParams::DefaultResponseParam,
		TArray<AActor*>(),
		false

	);

	// Calculate OutLaunchVelocity
	if (bHaveAimSolution)
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal(); // Turn velocity into unit vector to get launch angle
		MoveBarrelTowards(AimDirection);
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel) || !ensure(Turret)) { return; }
	
	// Work out difference between current barrel rotation and aim direction
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotation = AimAsRotator - BarrelRotator;
	
	Barrel->Elevate(DeltaRotation.Pitch);

	// Always rotate the shortest way
	if (FMath::Abs(DeltaRotation.Yaw) < 180)
	{
		Turret->Rotate(DeltaRotation.Yaw);
	}
	else
	{
		Turret->Rotate(-DeltaRotation.Yaw);
	}
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (ensure(Barrel))
	{
		FVector BarrelForward = Barrel->GetForwardVector();
		return !BarrelForward.Equals(AimDirection, 0.1);
	}
	else { return false; }
}

void UTankAimingComponent::Fire()
{
	if (FiringState == EFiringState::LOCKED || FiringState == EFiringState::AIMING)
	{
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBlueprint)) { return; }
		//Spawn a projectile at the socket location on the barrel
		FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
		RoundsLeft--;
	}
}



	

