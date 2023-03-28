#include "ElevatorButton.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"

AElevatorButton::AElevatorButton()
{
	PrimaryActorTick.bCanEverTick = false;
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = ButtonMesh;
	PlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlateMesh"));
}

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
		UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
		if (GameInstance)
		{
			GameInstance->SavePlayerData();
		}
		if (GetWorld()->GetMapName() != "Level3")
		{
			bIsPressed = true;
			UGameplayStatics::OpenLevel(this, LevelToOpen);
		}
	}
}
