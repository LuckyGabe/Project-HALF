
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectHALFGameModeBase.generated.h"

UCLASS()

class PROJECTHALF_API AProjectHALFGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectHALFGameModeBase();

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
	FString CurrentMapName;
	UPROPERTY()
		class UUserWidget* LoadingScreen;

	/* Handle to manage the timer */
	FTimerHandle LoadingScreenTimerHandle;

	void RemoveLoadingScreen();
};
