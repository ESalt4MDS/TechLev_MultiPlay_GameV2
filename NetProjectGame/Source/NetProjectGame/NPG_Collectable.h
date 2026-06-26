// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPG_Collectable.generated.h"
//#include "NPG_PlayerState.h"

UCLASS()
class NETPROJECTGAME_API ANPG_Collectable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPG_Collectable();

	void UpdatePlayerScore(class ANPG_PlayerState* _PlayerCollected);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Collectable")
	UStaticMeshComponent* CollectableMesh = nullptr;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
