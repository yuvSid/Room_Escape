// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG( LogTemp, Warning, TEXT( "Grabber reporting for duty" ) );
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// Get players view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( 
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation 
	);
	//UE_LOG( LogTemp, Warning, TEXT( "Location: %s\tPosition %s" ), 
	//	*PlayerViewPointLocation.ToString(), 
	//	*PlayerViewPointRotation.ToString() 
	//);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor( 255, 0, 0 ),
		false,
		0,
		0,
		10.f
	);

	/// Setup query params
	FCollisionQueryParams TraceParameters( FName(TEXT("")), false, GetOwner() );

	///Line-trace (AKA ray-cast) out to reach distance
	FHitResult Hit;
	
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams( ECollisionChannel::ECC_PhysicsBody ),
		TraceParameters
	);

	AActor * HittedActor = Hit.GetActor();
	if ( HittedActor )
		UE_LOG( LogTemp, Warning, TEXT("Line trace hit: %s"), *( HittedActor->GetName() ) )

	///See what we hit
	
}

