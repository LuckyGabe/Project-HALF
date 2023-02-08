// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadableNote.h"
#include <Blueprint/UserWidget.h>
#include "Kismet/GameplayStatics.h"
#include "ProjectHALFPlayerController.h"
// Sets default values
AReadableNote::AReadableNote()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	//RootComponent = BoxComponent;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(root);
	NoteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	NoteMesh->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AReadableNote::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<AProjectHALFPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	NoteMesh->SetWorldLocation(RootComponent->GetComponentLocation());
	NoteMesh->OnBeginCursorOver.AddDynamic(this, &AReadableNote::ShowPickUpMessage);


}

// Called every frame
void AReadableNote::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(player!=nullptr)
	{
	
		if (PlayerPos != player->GetActorLocation()) { CloseNote(); }
	
	}

}

void AReadableNote::OpenNote()
{

	if (NoteClass)
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		Note = CreateWidget<UUserWidget>(controller, NoteClass);
		Note->AddToViewport();

		bIsOpened = true;
	}
	
}

void AReadableNote::CloseNote()
{
	if (Note)
	{
	
		Note->RemoveFromViewport();
		bIsOpened = false;
		player = NULL;
	}

}

bool AReadableNote::IsOpened() { return bIsOpened; }

void AReadableNote::ShowPickUpMessage(UPrimitiveComponent* TouchedComponent)
{
	if (PlayerController)
	{

		PlayerController->bShowPickUpMessage = true;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AReadableNote::HidePickUpMessage, 1.f, false);
	}
}

void AReadableNote::HidePickUpMessage()
{
	if (PlayerController)
	{

		PlayerController->bShowPickUpMessage = false;
	}
}