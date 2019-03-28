// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
	
}

//Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if ( PhysicsHandle == nullptr )
	{
		UE_LOG( LogTemp, Error, TEXT( "%s missing physics handle component" ), *GetOwner()->GetName() );
	}
}

// Setup (assumed) attached input component
void UGrabber::SetupInputComponent()
{
	///Look for attached Input Handle (only appears at run time)
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if ( InputComponent )
	{
		InputComponent->BindAction( "Grab", IE_Pressed, this, &UGrabber::Grab );
		InputComponent->BindAction( "Grab", IE_Released, this, &UGrabber::Release );
	}
	else
	{
		UE_LOG( LogTemp, Error, TEXT( "%s missing input component" ), *GetOwner()->GetName() );
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	// if the physics handle is attached
	if ( PhysicsHandle->GrabbedComponent )
	{
		// move the object that we're holding
		PhysicsHandle->SetTargetLocation( GetReachLineEnd() );
	}
}

void UGrabber::Grab()
{
	// Line trace and see if we reach any actors with physics body collision channel set
	FHitResult HitResult = GetFitstPhysicsBodyInReach();
	UPrimitiveComponent * ComponentToGrab = HitResult.GetComponent();
	AActor * ActorHit = HitResult.GetActor();

	// If we hit something then attacg a physics handle
	if ( ActorHit )
	{
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation()
		);
	}
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFitstPhysicsBodyInReach() const
{
	///Line-trace (AKA ray-cast) out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters( FName( TEXT( "" ) ), false, GetOwner() );
	//FVector &LineStart = GetReachLineStart();
	//FVector &LineEnd = GetReachLineEnd();


	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams( ECollisionChannel::ECC_PhysicsBody ),
		TraceParameters
	);
	return HitResult;
}

FVector UGrabber::GetReachLineStart() const
{
	/// Get players view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() const
{
	/// Get players view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

