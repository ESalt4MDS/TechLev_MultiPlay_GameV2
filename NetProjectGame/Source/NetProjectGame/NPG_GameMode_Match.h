// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "NPG_GameMode_Match.generated.h"

/**
 * 
 */
UCLASS()
class NETPROJECTGAME_API ANPG_GameMode_Match : public AGameMode
{
	GENERATED_BODY()
	
protected:

	virtual void HandleMatchHasEnded() override;

	FTimerHandle MatchRestartDelayTimer;

public:

	void RespawnPlayer(APlayerController* _PlayerToRespawn);

	void CheckForWinner();


};
