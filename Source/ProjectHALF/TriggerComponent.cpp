// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ElevatorButton.h"
UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called every frame
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(GetAllowedActor()!= nullptr)
	{

		EnableElevator();
		GetAllowedActor()->SetActorLocation(GetComponentLocation() - FVector(0,0,15));
		GetAllowedActor()->SetActorRotation(FRotator(0, 0, 0));
		GetAllowedActor()->DisableComponentsSimulatePhysics();
	}
	

}


AActor* UTriggerComponent::GetAllowedActor() const
{

	TArray<AActor*> overlappingActors;
	GetOverlappingActors(overlappingActors);
	for(AActor* actor : overlappingActors)
	{
	
		if(actor->ActorHasTag("Key"))
		{
			
			return actor;
		
		}
	}
	return nullptr;
}

void UTriggerComponent::EnableElevator()
{

	AElevatorButton* button = Cast<AElevatorButton>(GetOwner());
	
					if (button != nullptr)
					{
						button->bHasPower = true;
					}


}