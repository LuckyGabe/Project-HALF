// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectHALFPlayerController.h"
#include "ElevatorButton.h"
UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called every frame
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bForGameEnd && GetAllowedActor() != nullptr)
	{
		EnableElevator(); //cast to button and enable power
		//Set key's location and rotation to fit to the power socket and lock it
		GetAllowedActor()->SetActorLocation(GetComponentLocation() - FVector(0, 0, 15));
		GetAllowedActor()->SetActorRotation(FRotator(0, 0, 0));
		GetAllowedActor()->DisableComponentsSimulatePhysics();
	}
	if (bForGameEnd && IsOverlappingPlayer()) { EndGame(); }

}

AActor* UTriggerComponent::GetAllowedActor() const
{
	TArray<AActor*> overlappingActors;
	GetOverlappingActors(overlappingActors);
	for (AActor* actor : overlappingActors)
	{
		if (actor->ActorHasTag("Key"))
		{
			return actor;
		}
	}
	return nullptr;
}

bool UTriggerComponent::IsOverlappingPlayer()
{
	TArray<AActor*> overlappingActors;
	GetOverlappingActors(overlappingActors);
	for (AActor* actor : overlappingActors)
	{
		if (actor->ActorHasTag("Player"))
		{
			return true;
		}
	}
	return false;
}

void UTriggerComponent::EndGame()
{
	AProjectHALFPlayerController* controller = Cast<AProjectHALFPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	controller->bGameWin = true;
}

void UTriggerComponent::EnableElevator()
{
	AElevatorButton* button = Cast<AElevatorButton>(GetOwner());

	if (button != nullptr)
	{
		button->bHasPower = true;
		if (button->PlateMesh)
		{
			button->PlateMesh->SetHiddenInGame(false);
		}
	}
}