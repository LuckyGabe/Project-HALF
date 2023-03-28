#include "ReadableNote.h"
#include <Blueprint/UserWidget.h>
#include "Kismet/GameplayStatics.h"
#include "ProjectHALFPlayerController.h"

AReadableNote::AReadableNote()
{
    PrimaryActorTick.bCanEverTick = true;

    root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    SetRootComponent(root);

    NoteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    NoteMesh->SetupAttachment(RootComponent);
    NoteMesh->OnBeginCursorOver.AddDynamic(this, &AReadableNote::ShowPickUpMessage);
}

void AReadableNote::BeginPlay()
{
    Super::BeginPlay();
    PlayerController = Cast<AProjectHALFPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    Note = CreateWidget<UUserWidget>(PlayerController, NoteClass);
}

void AReadableNote::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (player && PlayerPos != player->GetActorLocation())
    {
        CloseNote();
    }
}

void AReadableNote::OpenNote()
{
    if (NoteClass)
    {
        Note->AddToViewport();
        if (PageSound)
        {
            UGameplayStatics::PlaySound2D(GetWorld(), PageSound);
        }
        bIsOpened = true;
    }
}

void AReadableNote::CloseNote()
{
    if (Note)
    {
        Note->RemoveFromViewport();
        bIsOpened = false;
        player = nullptr;
    }
}

bool AReadableNote::IsOpened()
{
    return bIsOpened;
}

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
