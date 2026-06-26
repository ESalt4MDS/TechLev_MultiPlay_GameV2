// Fill out your copyright notice in the Description page of Project Settings.


#include "NPG_PlayerState.h"
#include "NPG_GameMode_Match.h"
#include "Net/UnrealNetwork.h"

void ANPG_PlayerState::GiveElimination()
{
	Eliminations++;

	if (ANPG_GameMode_Match* GameMode = GetWorld()->GetAuthGameMode<ANPG_GameMode_Match>())
	{
		if (GameMode->IsMatchInProgress())
		{
			GameMode->CheckForWinner();
		}
	}
}

int ANPG_PlayerState::GetEliminations()
{
	return Eliminations;
}

void ANPG_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANPG_PlayerState, Eliminations);


}