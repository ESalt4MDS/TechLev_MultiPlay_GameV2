// Fill out your copyright notice in the Description page of Project Settings.


#include "NPG_GameInstance.h"
#include <Online/OnlineSessionNames.h>

void UNPG_GameInstance::Init()
{
	Super::Init();

	GEngine->OnNetworkFailure().AddUObject(this, &UNPG_GameInstance::NetworkFailureHappened);

	if (IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get())
	{
		SessionInterface = OnlineSub->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNPG_GameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNPG_GameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNPG_GameInstance::OnJoinSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UNPG_GameInstance::OnDestroySessionComplete);

		}
	}
}

bool UNPG_GameInstance::HostSession(TSharedPtr<const FUniqueNetId> _userID, FName _sessionName, bool _bIsLan, bool _bIsPresence, int32 _maxNumPlayers)
{
	if (!SessionInterface.IsValid() || !_userID.IsValid()) return false;

	SessionSettings = MakeShareable(new FOnlineSessionSettings());

	SessionSettings->bIsLANMatch = _bIsLan;
	SessionSettings->bUsesPresence = _bIsPresence;
	SessionSettings->NumPublicConnections = _maxNumPlayers;
	SessionSettings->NumPrivateConnections = 0;
	SessionSettings->bAllowInvites = true;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
	SessionSettings->bUseLobbiesIfAvailable = true;

	//SessionSettings->Set(SETTING_MAPNAME, FString("Lvl_ThirdPerson"), EOnlineDataAdvertisementType::ViaOnlineService);
	SessionSettings->Set(SEARCH_KEYWORDS, FString("Custom"), EOnlineDataAdvertisementType::ViaOnlineService);

	return SessionInterface->CreateSession(*_userID, _sessionName, *SessionSettings);
}

void UNPG_GameInstance::OnCreateSessionComplete(FName _sessionName, bool _bSuccess)
{
	if (_bSuccess)
	{
		FString LevelOptions = "Lvl_ThirdPerson?listen";
		if (SessionSettings->bIsLANMatch) LevelOptions.Append("?bIsLanMatch=1");
		GetWorld()->ServerTravel(LevelOptions);
	}
}

void UNPG_GameInstance::FindSessions(TSharedPtr<const FUniqueNetId> _userID, bool _bIsLan)
{
	if (!SessionInterface.IsValid() || !_userID.IsValid()) OnFindSessionComplete(false);

	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	SessionSearch->bIsLanQuery = _bIsLan;
	SessionSearch->MaxSearchResults = 10000; //high value because a lot of people use the 480 code.
	SessionSearch->PingBucketSize = 50;
	SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
	SessionSearch->QuerySettings.Set(SEARCH_KEYWORDS, FString("Custom"), EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(*_userID, SessionSearch.ToSharedRef());

}

void UNPG_GameInstance::OnFindSessionComplete(bool _bSuccess)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, 
		FString::Printf(TEXT("OFindSessionsComplete bSuccess: %d"), _bSuccess));

	if (!SessionInterface.IsValid()) return;

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, 
		FString::Printf(TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num()));
	
	if (SessionSearch->SearchResults.Num() > 0)
	{
		for (int i = 0; i < SessionSearch->SearchResults.Num(); i++)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, 
				FString::Printf(TEXT("Session Number: %d | Sessionname: %s "), 
				i + 1, *(SessionSearch->SearchResults[i].Session.OwningUserName)));

		}
	}
}

bool UNPG_GameInstance::JoinSession(TSharedPtr<const FUniqueNetId> _userID, FName _sessionName, const FOnlineSessionSearchResult& _searchResult)
{
	if (!SessionInterface.IsValid()) return false;

	return SessionInterface->JoinSession(*_userID, _sessionName, _searchResult);
}

void UNPG_GameInstance::OnJoinSessionComplete(FName _sessionName, EOnJoinSessionCompleteResult::Type _result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
		FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), 
			*_sessionName.ToString(), static_cast<int32>(_result)));

	if (!SessionInterface.IsValid()) return;

	//get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map.
	APlayerController* const PlayerController = GetFirstLocalPlayerController();

	//we need a FString to use ClientTravel, and we can let the SessionInterface
	//construct it for us by giving it the SessionName and an empty String.
	//we want to do this because every OnlineSubsystem uses different TravelURLs.
	FString TravelURL;

	//finally call clientTravel
	if (PlayerController && SessionInterface->GetResolvedConnectString(_sessionName, TravelURL))
	{
		PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
	}
}

void UNPG_GameInstance::DestroySession()
{
	if (!SessionInterface.IsValid()) return;

	SessionInterface->DestroySession(NAME_GameSession);
}

void UNPG_GameInstance::OnDestroySessionComplete(FName _sessionName, bool _bSuccess)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
		FString::Printf(TEXT("OnDestroySessionComplete %s, %d"),
			*_sessionName.ToString(), _bSuccess));

	if (!SessionInterface.IsValid()) return;

	//always load the menu. Successful will be false if we try to leave during a singleplayer game, but we should still leave
	UGameplayStatics::OpenLevel(GetWorld(), "L_MainMenu", true);
}


void UNPG_GameInstance::StartGame(bool _bLan)
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	//TODO: change to false for steam, true for LAN
	HostSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), NAME_GameSession, false, true, 8);
}

void UNPG_GameInstance::FindGames(bool _bLan)
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	//TODO: change to false for steam, true for LAN
	FindSessions(Player->GetPreferredUniqueNetId().GetUniqueNetId(), false);
}

void UNPG_GameInstance::JoinGame(int _iServerIndex)
{
	//You could also pass in an entire SearchResult if we output them to the HUD at the 
	// end of the FindSessions step.

	ULocalPlayer* const Player = GetFirstGamePlayer();

	FOnlineSessionSearchResult SearchResult;

	if (SessionSearch->SearchResults.Num() > _iServerIndex)
	{
		SearchResult = SessionSearch->SearchResults[_iServerIndex];

		JoinSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), NAME_GameSession, SearchResult);
	}
}

void UNPG_GameInstance::NetworkFailureHappened(UWorld* _world, UNetDriver* _newDriver, ENetworkFailure::Type _failureType, const FString& _errorString)
{
	DestroySession();
}
