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
	CurrentMapName = GetWorld()->GetMapName(); //get current map name

	GetViewportSize(screenSizeX, ScreenSizeY);

	bGameOver = false;
	bGamePaused = false;


	//Add HUD to the viewport. There are two different map names limited because they differ after build and in the editor
	if (HUDClass && CurrentMapName != "MainMenu" && CurrentMapName != "UEDPIE_0_MainMenu")
	{
		PlayerHUD = CreateWidget<UUserWidget>(this, HUDClass);

		GetWorldTimerManager().SetTimer(LoadingScreenTimerHandle, this, &AProjectHALFPlayerController::DisplayHUD, 2.f, false);
	}
	//Create Widgets
	if (PickUpMessageClass)
	{
		PickUpMessage = CreateWidget<UUserWidget>(this, PickUpMessageClass);
	}

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
	// Main menu map has 2 names because they differ in the editor and after building the game
	if (bLockMouse && CurrentMapName != "MainMenu" && CurrentMapName != "UEDPIE_0_MainMenu") { SetMouseLocation(screenSizeX / 2, ScreenSizeY / 2); }

	//Pausing the game
	if (PauseMenu)
	{
		if (bGamePaused && !PauseMenu->IsInViewport())
		{
			SetShowMouseCursor(true);
			bLockMouse = false;
			PauseMenu->AddToViewport(); // add pause menu to the viewport
			SetPause(bGamePaused); //pause game
		}
		else if (!bGamePaused && PauseMenu->IsInViewport())
		{
			SetShowMouseCursor(false);

			PauseMenu->RemoveFromViewport();// remove pause menu from the viewport
			SetPause(bGamePaused); //unpause the game
			bLockMouse = true;
		}
	}
	if (GameOver)
	{
		if (bGameOver && !GameOver->IsInViewport())
		{
			SetShowMouseCursor(true);

			GameOver->AddToViewport(); // add game over screen to the viewport
			SetPause(bGameOver); //pause game
			bLockMouse = false;
		}
		else if (!bGameOver && GameOver->IsInViewport())
		{
			bShowMouseCursor = false;
			GameOver->RemoveFromViewport();
			SetPause(bGameOver); //unpause the game
			bLockMouse = true;
		}

	}

	if (GameWin)
	{
		if (bGameWin && !GameWin->IsInViewport())
		{
			SetShowMouseCursor(true);

			GameWin->AddToViewport(); // add game over screen to the viewport
			SetPause(bGameWin); //pause game
			bLockMouse = false;
		}

		else if (!GameWin && GameWin->IsInViewport())
		{
			bShowMouseCursor = false;
			GameWin->RemoveFromViewport();
			SetPause(bGameWin); //unpause the game
			bLockMouse = true;
		}

	}
	if (PickUpMessage)
	{
		if (bShowPickUpMessage && !PickUpMessage->IsInViewport())
		{


			PickUpMessage->AddToViewport(); // add game over screen to the viewport
		}

		else if (!bShowPickUpMessage && PickUpMessage->IsInViewport())
		{

			PickUpMessage->RemoveFromViewport();

		}

	}

}

void AProjectHALFPlayerController::DisplayHUD() { PlayerHUD->AddToViewport(); }
