// Fill out your copyright notice in the Description page of Project Settings.
//************************************************************************
//Bachelor of Software Engineering
//Media Design School
//Auckland
//New Zealand
//
//File Name : [NPG_Collectable.h]
//Description : [a actor that increases the players score in interacted with]
//Author : [Elijah Salt]
//Email : [elijah.salt@mds.ac.nz]
//
//(c) Elijah Salt
//************************************************************************

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPG_Collectable.generated.h"

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
