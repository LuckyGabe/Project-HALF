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
		FVector MoveOffSet;

	UPROPERTY(EditAnywhere)
		FVector OriginalLocation;
	UPROPERTY(EditAnywhere)
		FVector BoxCheckSize = FVector(300, 400, 400);


	UPROPERTY(EditAnywhere)
		float MoveTime = 2;
};

