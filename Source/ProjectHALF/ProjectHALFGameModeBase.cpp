
#include "ProjectHALFGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "PlayerCharacter.h"
#include <Blueprint/UserWidget.h>
AProjectHALFGameModeBase::AProjectHALFGameModeBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}


// Called when the game starts or when spawned
void AProjectHALFGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentMapName = GetWorld()->GetMapName(); //get current map name
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); //get player controller
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance()); //get game instance
	PlayerController->bShowMouseCursor = false; //Hide mouse cursor

	//play background music
	if (BackgroundMusic)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), BackgroundMusic);
	}

		//Load player data
	if (GameInstance && CurrentMapName != "MainMenu" && CurrentMapName != "UEDPIE_0_MainMenu")
	{
		GameInstance->LoadPlayerData();

	}
	// Main menu map has 2 names because they differ in the editor and after building the game
	if (CurrentMapName == "MainMenu" || CurrentMapName == "UEDPIE_0_MainMenu")
	{
		PlayerController->SetShowMouseCursor(true);

		GameInstance->ClearPlayerData();
	}

	// If it is a game level (not main menu) add loading screen to viewport and remove it after 2 seconds
	else
	{
		if (LoadingScreenClass)
		{
			LoadingScreen = CreateWidget<UUserWidget>(PlayerController, LoadingScreenClass);
			LoadingScreen->AddToViewport();
			GetWorldTimerManager().SetTimer(LoadingScreenTimerHandle,this, &AProjectHALFGameModeBase::RemoveLoadingScreen, 2.f, false);
		}
	}

	//if it is not the first level (and not main menu because if it was, cast would fail) spawn player's gun
	if (CurrentMapName != "Level1")
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (Player)
		{
			Player->SpawnGun();
		}
	}

	if (CurrentMapName == "Level2" && Level2StartEffects)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), Level2StartEffects);
	}


}

//add loading screen to viewport
void AProjectHALFGameModeBase::AddLoadingScreenToViewport()
{
	if (LoadingScreen)
	{
		LoadingScreen->AddToViewport();
	}

}


//add loading screen from viewport
void AProjectHALFGameModeBase::RemoveLoadingScreen()
{
	if (LoadingScreen)
	{
		LoadingScreen->RemoveFromViewport();
	}


}
