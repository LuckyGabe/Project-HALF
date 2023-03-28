#include "Pickable.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectHALFPlayerController.h"

APickable::APickable()
{
	PrimaryActorTick.bCanEverTick = false;
	bActive = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	Mesh->OnBeginCursorOver.AddDynamic(this, &APickable::ShowPickUpMessage);
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
	if (bActive && PlayerController && !PlayerController->bShowPickUpMessage)
	{
		PlayerController->bShowPickUpMessage = true;
		FTimerHandle timerHandle;
		GetWorldTimerManager().SetTimer(timerHandle, this, &APickable::HidePickUpMessage, 0.7f, false);
	}
}

void APickable::HidePickUpMessage()
{
	if (PlayerController && PlayerController->bShowPickUpMessage)
	{
		PlayerController->bShowPickUpMessage = false;
	}
}

void APickable::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<AProjectHALFPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}
