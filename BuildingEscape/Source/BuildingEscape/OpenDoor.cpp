// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger Volume
	if ( GetTotalMassOfActorsOnPlate() > MassTrigerPlate )	
	{
		OpenDoor();
		LastOpenDoorTime = GetWorld()->GetTimeSeconds();
	}

	// Checks if time delay is over and door must be closed
	if ( GetWorld()->GetTimeSeconds() - LastOpenDoorTime > DoorCloseDelay )
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// Find all the overlapping actors
	TArray< AActor * > OverlappingActors;
	PressurePlate->GetOverlappingActors( OUT OverlappingActors );

	// Iterate through them assing their masses
	for ( const auto* EachActor : OverlappingActors )
	{
		TotalMass += EachActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG( LogTemp, Warning, TEXT( "%s on pressure plate" ), *EachActor->GetName() );
	}

	return TotalMass;
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(
		FRotator( 0.f, OpenAngle, 0.f ),
		ETeleportType::None );
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(
		FRotator( 0.f, 0.f, 0.f ),
		ETeleportType::None );
}

