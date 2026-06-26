// Copyright Epic Games, Inc. All Rights Reserved.


#include "NetProjectGamePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "NetProjectGame.h"
#include "Widgets/Input/SVirtualJoystick.h"
#include "NPG_GameMode_Match.h"

void ANetProjectGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogNetProjectGame, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void ANetProjectGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

bool ANetProjectGamePlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}

void ANetProjectGamePlayerController::RespawnAfterDelay()
{
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &ANetProjectGamePlayerController::TryRespawn, 5.0f, false);
}

void ANetProjectGamePlayerController::TryRespawn()
{
	if (ANPG_GameMode_Match* GameMode = Cast<ANPG_GameMode_Match>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->RespawnPlayer(this);
	}
}

