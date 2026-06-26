// Fill out your copyright notice in the Description page of Project Settings.


#include "NPG_Cube.h"

// Sets default values
ANPG_Cube::ANPG_Cube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>("Cube Mesh");
	RootComponent = CubeMesh;

	bReplicates = true;
}

// Called when the game starts or when spawned
void ANPG_Cube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPG_Cube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

