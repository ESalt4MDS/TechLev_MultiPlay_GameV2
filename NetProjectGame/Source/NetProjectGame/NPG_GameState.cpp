// Fill out your copyright notice in the Description page of Project Settings.


#include "NPG_GameState.h"
#include "NPG_PlayerState.h"
#include "Net/UnrealNetwork.h"


void ANPG_GameState::SetWinner(ANPG_PlayerState* _Winner)
{
	WinningPlayer = _Winner;
}

ANPG_PlayerState* ANPG_GameState::GetWinner()
{
	return WinningPlayer;
}

void ANPG_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANPG_GameState, WinningPlayer);


}
