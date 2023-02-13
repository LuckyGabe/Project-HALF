// Fill out your copyright notice in the Description page of Project Settings.


#include "ElevatorButton.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectHALFPlayerController.h"
#include "MyGameInstance.h"
// Sets default values
AElevatorButton::AElevatorButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = ButtonMesh;
}

void AElevatorButton::Press()
{
	if (bHasPower)
	{
		UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance()); //get game instance
		if (GameInstance)
		{
			//save player data
			GameInstance->SavePlayerData();
		}

		if (LevelToOpen == "Level3")
		{
			AProjectHALFPlayerController* controller = Cast<AProjectHALFPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			controller->bGameWin = true;

		}

		if (GetWorld()->GetMapName() != "Level2")
		{
			bIsPressed = true;
			// load next level
			UGameplayStatics::OpenLevel(this, LevelToOpen);

		}

	}


}
