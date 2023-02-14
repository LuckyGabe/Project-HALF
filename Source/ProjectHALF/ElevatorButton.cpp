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
	PlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlateMesh"));
}
// Called when the game starts or when spawned
void AElevatorButton::BeginPlay()
{
	Super::BeginPlay();
	if (PlateMesh)
	{
		PlateMesh->SetHiddenInGame(true);
	}
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

		if (GetWorld()->GetMapName() != "Level3")
		{
			bIsPressed = true;
			// load next level
			UGameplayStatics::OpenLevel(this, LevelToOpen);

		}

	}


}
