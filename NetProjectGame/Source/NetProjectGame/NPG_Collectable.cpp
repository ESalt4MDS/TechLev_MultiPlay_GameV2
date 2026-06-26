// Fill out your copyright notice in the Description page of Project Settings.


#include "NPG_Collectable.h"
#include "NPG_PlayerState.h"

// Sets default values
ANPG_Collectable::ANPG_Collectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	CollectableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollectableMesh"));
	RootComponent = CollectableMesh;

	bReplicates = true;

}

/* Updates players score and destroys its self */
void ANPG_Collectable::UpdatePlayerScore(ANPG_PlayerState* _PlayerCollected)
{
	_PlayerCollected->GiveScore();

	if (HasAuthority())
	{
		Destroy();
	}
}

// Called when the game starts or when spawned
void ANPG_Collectable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPG_Collectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

