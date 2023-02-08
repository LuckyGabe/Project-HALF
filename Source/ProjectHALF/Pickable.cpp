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
	Mesh->OnBeginCursorOver.AddDynamic(this, &APickable::ShowPickUpMessage);
}

// Called every frame
void APickable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickable::PickedUp()
{
	bActive = false;
	SetActorHiddenInGame(true);
}

bool APickable::GetActive()
{
	return bActive;
}

void APickable::ShowPickUpMessage(UPrimitiveComponent* TouchedComponent)
{
	if (bActive && PlayerController) 
	{

		PlayerController->bShowPickUpMessage = true;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &APickable::HidePickUpMessage, 1.f, false);
	}
}

void APickable::HidePickUpMessage()
{
	if (PlayerController)
	{
	
	PlayerController->bShowPickUpMessage = false;
	}
}

