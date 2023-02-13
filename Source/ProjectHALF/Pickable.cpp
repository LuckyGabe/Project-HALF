// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectHALFPlayerController.h"
// Sets default values
APickable::APickable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bActive = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;

}

// Called when the game starts or when spawned
void APickable::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AProjectHALFPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	//Call the function ShowPickUpMessage() when the mouse hover over this Mesh
	Mesh->OnBeginCursorOver.AddDynamic(this, &APickable::ShowPickUpMessage);
}

void APickable::PickedUp()
{
	bActive = false;
	SetActorHiddenInGame(true); //hide this actor

}

bool APickable::GetActive()
{
	return bActive;
}

void APickable::ShowPickUpMessage(UPrimitiveComponent* TouchedComponent)
{
	if (bActive && PlayerController)
	{
		// set the bool in player controller to add the message widget to the viewport
		PlayerController->bShowPickUpMessage = true;
		//after 0.7s hide the pick up message
		GetWorldTimerManager().SetTimer(TimerHandle, this, &APickable::HidePickUpMessage, 0.7f, false);
	}
}

void APickable::HidePickUpMessage()
{
	if (PlayerController)
	{
		// set the bool in player controller to remove the message widget from the viewport
		PlayerController->bShowPickUpMessage = false;
	}
}

