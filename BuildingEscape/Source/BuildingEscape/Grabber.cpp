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

// Setup (assumed) attached input component
void UGrabber::SetupInputComponent()
{
	///Look for attached Input Handle (only appears at run time)
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if ( InputComponent )
	{
		UE_LOG( LogTemp, Warning, TEXT( "%s input component found successfully" ), *GetOwner()->GetName() );
		///Bind the input axis
		InputComponent->BindAction( "Grab", IE_Pressed, this, &UGrabber::Grab );
		InputComponent->BindAction( "Grab", IE_Released, this, &UGrabber::Release );
	}
	else
	{
		UE_LOG( LogTemp, Error, TEXT( "%s missing input component" ), *GetOwner()->GetName() );
	}
}

//Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if ( PhysicsHandle )
	{
		//Physics handle is found
	}
	else
	{
		UE_LOG( LogTemp, Error, TEXT( "%s missing physics handle component" ), *GetOwner()->GetName() );
	}
}


void UGrabber::Grab()
{
	UE_LOG( LogTemp, Warning, TEXT( "Grab pressed" ) );

	// Line trace and see if we reach any actors with physics body collision channel set
	FHitResult HitResult = GetFitstPhysicsBodyInReach();
	UPrimitiveComponent * ComponentToGrab = HitResult.GetComponent();
	AActor * ActorHit = HitResult.GetActor();

	// If we hit something then attacg a physics handle
	if ( ActorHit )
	{
		// TODO attach physics handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true
		);
	}	
}

void UGrabber::Release()
{
	UE_LOG( LogTemp, Warning, TEXT( "Grab released" ) );
	// TODO release physics handle
	PhysicsHandle->ReleaseComponent();
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

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	
	// if the physics handle is attached
	if ( PhysicsHandle->GrabbedComponent )
	{
		// move the object that we're holding
		PhysicsHandle->SetTargetLocation( LineTraceEnd );
	}
}

FHitResult UGrabber::GetFitstPhysicsBodyInReach() const
{
	/// Get players view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	/// Setup query params
	FCollisionQueryParams TraceParameters( FName( TEXT( "" ) ), false, GetOwner() );

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
		UE_LOG( LogTemp, Warning, TEXT( "Line trace hit: %s" ), *( HittedActor->GetName() ) )

		///See what we hit

		return Hit;
}



