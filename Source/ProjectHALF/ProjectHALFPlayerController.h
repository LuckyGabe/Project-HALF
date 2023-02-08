// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectHALFPlayerController.generated.h"

/**
 *
 */
UCLASS()
class PROJECTHALF_API AProjectHALFPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AProjectHALFPlayerController();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY()
		bool bGameOver = false;
	UPROPERTY()
		bool bGameWin = false;

	UPROPERTY()
		bool bGamePaused = false;

	UPROPERTY()
		bool bLockMouse = true;
	UPROPERTY()
		bool bShowPickUpMessage = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> HUDClass;

	UPROPERTY()
		class UUserWidget* PlayerHUD;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> PauseMenuClass;

	UPROPERTY()
		class UUserWidget* PauseMenu;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> GameOverClass;

	UPROPERTY()
		class UUserWidget* GameOver;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> GameWinClass;

	UPROPERTY()
		class UUserWidget* GameWin;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> PickUpMessageClass;

	UPROPERTY()
		class UUserWidget* PickUpMessage;

	void DisplayHUD();
	int screenSizeX, ScreenSizeY;

	/* Handle to manage the timer */
	FTimerHandle LoadingScreenTimerHandle;
};
