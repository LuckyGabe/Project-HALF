#include "TriggerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectHALFPlayerController.h"
#include "ElevatorButton.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bForGameEnd && IsOverlappingPlayer())
	{
		EndGame();
		return;
	}

	if (bForGameEnd)
	{
		return;
	}

	AActor* AllowedActor = GetAllowedActor();

	if (AllowedActor != nullptr)
	{
		AElevatorButton* Button = Cast<AElevatorButton>(GetOwner());

		if (Button != nullptr)
		{
			Button->bHasPower = true;

		}

		FVector KeyLocation = GetComponentLocation() - FVector(0.f, 0.f, 15.f);
		FRotator KeyRotation = FRotator::ZeroRotator;
		AllowedActor->SetActorLocation(KeyLocation);
		AllowedActor->SetActorRotation(KeyRotation);
		AllowedActor->DisableComponentsSimulatePhysics();
	}
}

AActor* UTriggerComponent::GetAllowedActor() const
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor->ActorHasTag("Key"))
		{
			return OverlappingActor;
		}
	}

	return nullptr;
}

bool UTriggerComponent::IsOverlappingPlayer()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor->ActorHasTag("Player"))
		{
			return true;
		}
	}

	return false;
}

void UTriggerComponent::EndGame()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AProjectHALFPlayerController* ProjectHALFPlayerController = Cast<AProjectHALFPlayerController>(PlayerController);

	if (ProjectHALFPlayerController != nullptr)
	{
		ProjectHALFPlayerController->bGameWin = true;
	}
}
