// Fill out your copyright notice in the Description page of Project Settings.


#include "NPG_InteractableDoor.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ANPG_InteractableDoor::ANPG_InteractableDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>("Door Mesh");
	RootComponent = DoorMesh;

	bReplicates = true;

}

/* Set the actor to open or closed location (position) when interacted with by player */
void ANPG_InteractableDoor::ToggleDoor()
{
	IsOpen = !IsOpen;

	if (IsOpen)
	{
		SetActorLocation(OpenPosition);
	}
	else
	{
		SetActorLocation(ClosedPosition);
	}

}

// Called when the game starts or when spawned
void ANPG_InteractableDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPG_InteractableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}
