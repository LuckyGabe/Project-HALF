// Fill out your copyright notice in the Description page of Project Settings.


#include "Keypad.h"
#include "Kismet/GameplayStatics.h"
#include <Blueprint/UserWidget.h>
// Sets default values
AKeypad::AKeypad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(root);
	KeypadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	KeypadMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AKeypad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKeypad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (player != nullptr)
	{

		if (PlayerPos != player->GetActorLocation()) { CloseKeypad(); }

	}
}

void AKeypad::OpenKeypad()
{
	UE_LOG(LogTemp, Warning, TEXT("OpenNote"));
	if (KeypadClass)
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		Keypad = CreateWidget<UUserWidget>(controller, KeypadClass);
		Keypad->AddToViewport();

		controller->bShowMouseCursor = true;


		bIsOpened = true;
	}

}

void AKeypad::CloseKeypad()
{
	if (Keypad)
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		controller->bShowMouseCursor = false;
		Keypad->RemoveFromViewport();
		bIsOpened = false;
		player = NULL;
	}

}

bool AKeypad::IsOpened() { return bIsOpened; }
