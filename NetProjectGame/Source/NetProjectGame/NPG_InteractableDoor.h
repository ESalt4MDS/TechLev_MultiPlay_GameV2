// Fill out your copyright notice in the Description page of Project Settings.
//************************************************************************
//Bachelor of Software Engineering
//Media Design School
//Auckland
//New Zealand
//
//File Name : [NPG_InteractableDoor.h]
//Description : [door that opens and closes when interacted with]
//Author : [Elijah Salt]
//Email : [elijah.salt@mds.ac.nz]
//
//(c) Elijah Salt
//************************************************************************
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPG_InteractableDoor.generated.h"

UCLASS()
class NETPROJECTGAME_API ANPG_InteractableDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPG_InteractableDoor();

	void ToggleDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Door")
	UStaticMeshComponent* DoorMesh = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "Door")
	FVector ClosedPosition = FVector::ZeroVector;

	UPROPERTY(EditInstanceOnly, Category = "Door")
	FVector OpenPosition = FVector::ZeroVector;

	bool IsOpen = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
