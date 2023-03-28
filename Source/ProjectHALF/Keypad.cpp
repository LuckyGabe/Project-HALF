#include "Keypad.h"
#include "Kismet/GameplayStatics.h"
#include <Blueprint/UserWidget.h>

AKeypad::AKeypad()
{
	PrimaryActorTick.bCanEverTick = true;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(root);
	KeypadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	KeypadMesh->SetupAttachment(RootComponent);
}

void AKeypad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (player && PlayerPos != player->GetActorLocation())
	{
		CloseKeypad();
	}
}

void AKeypad::OpenKeypad()
{
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
		player = nullptr;
	}
}

bool AKeypad::IsOpened() const
{
	return bIsOpened;
}
