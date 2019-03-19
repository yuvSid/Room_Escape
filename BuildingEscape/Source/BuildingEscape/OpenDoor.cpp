// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"

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

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	Owner = GetOwner();	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if ( PressurePlate->IsOverlappingActor( ActorThatOpens ) )	
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

