// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "Components/PrimitiveComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FDoorEvent );

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY( BlueprintAssignable )
	FDoorEvent OnOpen;

	UPROPERTY( BlueprintAssignable )
	FDoorEvent OnClose;

private:
	UPROPERTY( EditAnywhere )
	ATriggerVolume * PressurePlate = nullptr;

	UPROPERTY( EditAnywhere )
	float MassTrigerPlate = 20.f;

	AActor * Owner = nullptr;

	float GetTotalMassOfActorsOnPlate();
		
};
