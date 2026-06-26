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

void ANPG_InteractableDoor::ToggleDoor()
{
	if (HasAuthority())
	{
		Destroy();
	}

	//IsOpen = !IsOpen;

	//if (HasAuthority())
	//{
	//	if (IsOpen)
	//	{
	//		//move the door towards the closed position
	//		DoorMesh->SetWorldLocation(FMath::VInterpTo(DoorMesh->GetComponentLocation(), ClosedPosition, DeltaTime, 2.0f));
	//	}
	//	else
	//	{
	//		//move the door towards the open position
	//		DoorMesh->SetWorldLocation(FMath::VInterpTo(DoorMesh->GetComponentLocation(), OpenPosition, DeltaTime, 2.0f));
	//	}
	//}

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

	//if (HasAuthority())
	//{
	//	if (IsOpen)
	//	{

	//		//move the door towards the closed position
	//		DoorMesh->SetWorldLocation(FMath::VInterpTo(DoorMesh->GetComponentLocation(), ClosedPosition, DeltaTime, 2.0f));

	//		//check if the door has reached the closed position
	//		//if (DoorMesh->GetComponentLocation().Equals(ClosedPosition, 1.0f))
	//		//{
	//		//	IsOpen = false;
	//		//}

	//	}
	//	else
	//	{

	//		//move the door towards the open position
	//		DoorMesh->SetWorldLocation(FMath::VInterpTo(DoorMesh->GetComponentLocation(), OpenPosition, DeltaTime, 2.0f));

	//		//check if the door has reached the open position
	//		//if (DoorMesh->GetComponentLocation().Equals(OpenPosition, 1.0f))
	//		//{
	//		//	IsOpen = true;
	//		//}

	//	}
	//}

}

void ANPG_InteractableDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANPG_InteractableDoor, IsOpen);

}

