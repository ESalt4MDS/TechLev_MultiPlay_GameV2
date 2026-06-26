// Fill out your copyright notice in the Description page of Project Settings.


#include "NPG_InteractableDoor.h"

// Sets default values
ANPG_InteractableDoor::ANPG_InteractableDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>("Door Mesh");
	RootComponent = DoorMesh;

	bReplicates = true;

}

void ANPG_InteractableDoor::ToggleDoor()
{
	IsOpen = !IsOpen;
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

	if (IsOpen)
	{
		if (HasAuthority())
		{
			//move the door towards the closed position
			DoorMesh->SetWorldLocation(FMath::VInterpTo(DoorMesh->GetComponentLocation(), ClosedPosition, DeltaTime, 2.0f));
			//check if the door has reached the closed position
			if (DoorMesh->GetComponentLocation().Equals(ClosedPosition, 1.0f))
			{
				IsOpen = false;
			}
		}
	}
	else
	{
		if (HasAuthority())
		{
			//move the door towards the open position
			DoorMesh->SetWorldLocation(FMath::VInterpTo(DoorMesh->GetComponentLocation(), OpenPosition, DeltaTime, 2.0f));

			//check if the door has reached the open position
			if (DoorMesh->GetComponentLocation().Equals(OpenPosition, 1.0f))
			{
				IsOpen = true;
			}
		}
	}

}

