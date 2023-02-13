// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickable.generated.h"

class UStaticMeshComponent;

UCLASS()
class PROJECTHALF_API APickable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	void PickedUp();
	bool GetActive();

private:
	bool bActive;
	/* Handle to manage the timer */
	FTimerHandle TimerHandle;
	class AProjectHALFPlayerController* PlayerController;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Mesh;
	UFUNCTION()
		void ShowPickUpMessage(UPrimitiveComponent* TouchedComponent);
	UFUNCTION()
		void HidePickUpMessage();
};
