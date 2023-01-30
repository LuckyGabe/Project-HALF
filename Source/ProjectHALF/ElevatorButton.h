// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElevatorButton.generated.h"

UCLASS()
class PROJECTHALF_API AElevatorButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElevatorButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool bIsPressed = false;
	void Press();
UPROPERTY(EditAnywhere)
	bool bHasPower = true;

private:
	UPROPERTY(EditAnywhere)
		FVector MoveOffSet;
	UPROPERTY()
		FVector OriginalLocation;
	UPROPERTY(EditAnywhere)
		float MoveTime = 2;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* ButtonMesh;

};
