// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectHALFPlayerController.h"
#include <Blueprint/UserWidget.h>


AProjectHALFPlayerController::AProjectHALFPlayerController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

	// Called when the game starts or when spawned
void AProjectHALFPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FString CurrentMapName = GetWorld()->GetMapName(); //get current map name
	//Add HUD to the viewport
	if (HUDClass && CurrentMapName != "MainMenu")
	{
		PlayerHUD = CreateWidget<UUserWidget>(this, HUDClass);
		
		GetWorldTimerManager().SetTimer(LoadingScreenTimerHandle, this, &AProjectHALFPlayerController::DisplayHUD, 2.f, false);
	}
	bGameOver = false;
	bGamePaused = false;
	
	if (PauseMenuClass)
	{
		PauseMenu = CreateWidget<UUserWidget>(this, PauseMenuClass);
		
	}
	if (GameOverClass)
	{
		GameOver = CreateWidget<UUserWidget>(this, GameOverClass);

	}

	if (GameWinClass)
	{
		GameWin = CreateWidget<UUserWidget>(this, GameWinClass);
	}

}

// Called every frame
void AProjectHALFPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Pausing the game
	if (PauseMenu)
	{
		if (bGamePaused && !PauseMenu->IsInViewport())
		{
			SetShowMouseCursor(true);

			PauseMenu->AddToViewport(); // add pause menu to the viewport
			SetPause(bGamePaused); //pause game
		}
		else if (!bGamePaused && PauseMenu->IsInViewport())
		{
			SetShowMouseCursor(false);

			PauseMenu->RemoveFromViewport();// remove pause menu from the viewport
			SetPause(bGamePaused);
		}
	}
	if (GameOver)
	{
		if (bGameOver && !GameOver->IsInViewport())
		{
			SetShowMouseCursor(true);

			GameOver->AddToViewport(); // add game over screen to the viewport
			SetPause(bGameOver); //pause game
		}
		else if (!bGameOver && GameOver->IsInViewport())
		{
			bShowMouseCursor = false;
			GameOver->RemoveFromViewport();
			SetPause(bGameOver);
		}

	}

	if (GameWin)
	{
		if (bGameWin && !GameWin->IsInViewport())
		{
			SetShowMouseCursor(true);

			GameWin->AddToViewport(); // add game over screen to the viewport
			SetPause(bGameWin); //pause game
		}

		else if (!GameWin && GameWin->IsInViewport())
		{
			bShowMouseCursor = false;
			GameWin->RemoveFromViewport();
			SetPause(bGameWin);
		}

	}


}
	
	void AProjectHALFPlayerController::DisplayHUD() { PlayerHUD->AddToViewport(); }
