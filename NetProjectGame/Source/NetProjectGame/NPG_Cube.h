// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPG_Cube.generated.h"

UCLASS()
class NETPROJECTGAME_API ANPG_Cube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPG_Cube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Cube") //clean up 
	UStaticMeshComponent* CubeMesh = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
