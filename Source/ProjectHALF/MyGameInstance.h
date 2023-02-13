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
	UFUNCTION()
	void SavePlayerData();
	UFUNCTION()
		void LoadPlayerData();
	UFUNCTION()
		void ClearPlayerData();
	//Store Player data
	float Ammo;
	float MagAmmo;
	int MedKitsNumb;
	UPROPERTY(EditAnywhere)
	float Health;
	UPROPERTY()
		bool bHasGun = false;
	UPROPERTY()
		class AGun* Gun;
	UPROPERTY()
	class	APlayerCharacter* Player;

};
