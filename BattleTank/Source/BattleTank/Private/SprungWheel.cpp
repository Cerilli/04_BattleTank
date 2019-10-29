// Fill out your copyright notice in the Description page of Project Settings.


#include "SprungWheel.h"
#include "Components\StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"


// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("PhysicsConstraint"));
	SetRootComponent(PhysicsConstraint);

	Wheel = CreateDefaultSubobject<UStaticMeshComponent>(FName("Wheel"));
	Wheel->SetupAttachment(PhysicsConstraint);}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
	SetupConstraint();
}

void ASprungWheel::SetupConstraint()
{
	if (!GetAttachParentActor()) { return; }
	UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());

	if (!BodyRoot) { return; }
	PhysicsConstraint->SetConstrainedComponents(BodyRoot, NAME_None, Wheel, NAME_None);

}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

