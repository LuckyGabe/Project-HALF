// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectHALFPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHALF_API AProjectHALFPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AProjectHALFPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
		UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> HUDClass;

		UPROPERTY()
			class UUserWidget* PlayerHUD;
};
