// Fill out your copyright notice in the Description page of Project Settings.
//************************************************************************
//Bachelor of Software Engineering
//Media Design School
//Auckland
//New Zealand
//
//File Name : [NPG_GameInstance.h]
//Description : [the game instance]
//Author : [Elijah Salt]
//Email : [elijah.salt@mds.ac.nz]
//
//(c) Elijah Salt
//************************************************************************
#pragma once

#include "NetProjectGame.h"
#include "Engine/GameInstance.h"
#include "NPG_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NETPROJECTGAME_API UNPG_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:

	void OnCreateSessionComplete(FName _sessionName, bool _bSuccess);
	void OnFindSessionComplete(bool _bSuccess);
	void OnJoinSessionComplete(FName _sessionName, EOnJoinSessionCompleteResult::Type _result);
	void OnDestroySessionComplete(FName _sessionName, bool _bSuccess);

	IOnlineSessionPtr SessionInterface;
	virtual void Init() override;

	TSharedPtr<FOnlineSessionSettings> SessionSettings;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	bool HostSession(TSharedPtr<const FUniqueNetId> _userID, FName _sessionName, bool _bIsLan, bool _bIsPresence, int32 _maxNumPlayers);
	void FindSessions(TSharedPtr<const FUniqueNetId> _userID, bool _bIsLan);
	bool JoinSession(TSharedPtr<const FUniqueNetId> _userID, FName _sessionName, const FOnlineSessionSearchResult& _searchResult);
	
	UFUNCTION(BlueprintCallable)
	void StartGame(bool _bLan);

	UFUNCTION(BlueprintCallable)
	void FindGames(bool _bLan);

	UFUNCTION(BlueprintCallable)
	void JoinGame(int _iServerIndex);

	UFUNCTION(BlueprintCallable)
	void DestroySession();

	UFUNCTION()
	void NetworkFailureHappened(UWorld* _world, UNetDriver* _newDriver, ENetworkFailure::Type _failureType, const FString& _errorString);

};
