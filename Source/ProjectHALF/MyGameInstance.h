// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHALF_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:
	float ammo;
	float magAmmo;
	UPROPERTY(EditAnywhere)
	float health;
	UPROPERTY()
		class AGun* Gun;

	UFUNCTION(BlueprintCallable)
	void MyFunction();
};
