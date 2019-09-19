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
	if ( !Owner )
		UE_LOG( LogTemp, Error, TEXT( "%s no owner provided" ), *GetOwner()->GetName() );
	if ( !PressurePlate )
		UE_LOG( LogTemp, Error, TEXT( "%s missing pressure plate setting" ), *GetOwner()->GetName() );
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger Volume
	if ( GetTotalMassOfActorsOnPlate() > MassTrigerPlate )	
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// Find all the overlapping actors
	TArray< AActor * > OverlappingActors;
	if ( !PressurePlate )	
		return TotalMass;
	PressurePlate->GetOverlappingActors( OUT OverlappingActors );

	// Iterate through them assing their masses
	for ( const auto* EachActor : OverlappingActors )
	{
		TotalMass += EachActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG( LogTemp, Warning, TEXT( "%s on pressure plate" ), *EachActor->GetName() );
	}

	return TotalMass;
}
