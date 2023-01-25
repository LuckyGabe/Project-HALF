// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadableNote.h"
#include <Blueprint/UserWidget.h>
#include "Kismet/GameplayStatics.h"
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
	NoteMesh->SetWorldLocation(RootComponent->GetComponentLocation());
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
	UE_LOG(LogTemp, Warning, TEXT("OpenNote"));
	if (NoteClass)
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		Note = CreateWidget<UUserWidget>(controller, NoteClass);
		Note->AddToViewport();

		controller->bShowMouseCursor = true;


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