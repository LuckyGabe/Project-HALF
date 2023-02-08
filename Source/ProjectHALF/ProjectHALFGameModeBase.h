// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectHALFGameModeBase.generated.h"

/**
 * 
 */

UCLASS()

class PROJECTHALF_API AProjectHALFGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectHALFGameModeBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void AddLoadingScreenToViewport();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		USoundBase* BackgroundMusic;
	UPROPERTY(EditAnywhere)
		USoundBase* Level2StartEffects;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> LoadingScreenClass;

	UPROPERTY()
		class UUserWidget* LoadingScreen;
	/* Handle to manage the timer */
	FTimerHandle LoadingScreenTimerHandle;
	void RemoveLoadingScreen();
};
