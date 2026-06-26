// Fill out your copyright notice in the Description page of Project Settings.


#include "NPG_GameMode_Match.h"
#include "NPG_GameState.h"
#include "NPG_PlayerState.h"

void ANPG_GameMode_Match::HandleMatchHasEnded()
{
	GetWorldTimerManager().SetTimer(MatchRestartDelayTimer, this, &AGameMode::RestartGame, 5.0f, false);
}

void ANPG_GameMode_Match::RespawnPlayer(APlayerController* _PlayerToRespawn)
{
	//respawn only if a match is in progress
	if (IsMatchInProgress())
	{
		// RestartPlayer(_PlayerToRespawn);

		int32 RandIndex = FMath::RandRange(0, SpawnLocations.Num() - 1);
		if (SpawnLocations.IsValidIndex(RandIndex))
		{
			FTransform SpawnTransform(SpawnLocations[RandIndex]);
			RestartPlayerAtTransform(_PlayerToRespawn, SpawnTransform);
		}
	}
}

void ANPG_GameMode_Match::CheckForWinner()
{
	//TODO: Check if any player has enough to win

	if (ANPG_GameState* NPGGameState = GetGameState<ANPG_GameState>())
	{
		for (auto iter : NPGGameState->PlayerArray)
		{
			if (ANPG_PlayerState* NPGPlayerState = Cast<ANPG_PlayerState>(iter))
			{
				if (NPGPlayerState->GetPlayerScore() >= 5)
				{
					NPGGameState->SetWinner(NPGPlayerState);

					EndMatch();

					break;
				}
			}
		}
	}
}
