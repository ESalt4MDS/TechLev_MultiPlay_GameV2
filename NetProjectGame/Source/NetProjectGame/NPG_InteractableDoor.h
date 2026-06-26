// Fill out your copyright notice in the Description page of Project Settings.

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

	UPROPERTY(Replicated);
	bool IsOpen = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
