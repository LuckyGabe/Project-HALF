// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DoorMover.generated.h"

class UBoxComponent;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTHALF_API UDoorMover : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDoorMover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite)
		bool bShouldMove = false;

private:

	AActor* Owner = GetOwner();
	AActor* GetAllowedActor() const;

	UPROPERTY(EditAnywhere)
		FVector MoveOffSet; //how far and which direction should the door move  

	float time = 0;

	UPROPERTY(EditAnywhere)
		FVector OriginalLocation; //start location (door closed)

	UPROPERTY(EditAnywhere)
		FVector BoxCheckSize = FVector(300, 400, 400); // size of the overlapping box (adjustable in the editor)

	UPROPERTY(EditAnywhere)
		float MoveTime = 1; // How long it take the doors to fully open

};

