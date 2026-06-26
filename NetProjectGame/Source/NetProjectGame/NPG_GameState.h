// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "NPG_GameState.generated.h"

/**
 * 
 */
UCLASS()
class NETPROJECTGAME_API ANPG_GameState : public AGameState
{
	GENERATED_BODY()

protected:

	UPROPERTY(Replicated)
	ANPG_PlayerState* WinningPlayer = nullptr;

public:
	
	void SetWinner(class ANPG_PlayerState* _Winner);

	UFUNCTION(BlueprintPure)
	ANPG_PlayerState* GetWinner();

	
};
