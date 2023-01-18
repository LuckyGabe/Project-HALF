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
	UFUNCTION()
	void ShowPickUpMessage(UPrimitiveComponent* TouchedComponent);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void PickedUp();
	bool GetActive();

private:
	bool bActive;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Mesh;
};
