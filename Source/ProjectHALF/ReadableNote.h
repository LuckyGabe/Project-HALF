// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReadableNote.generated.h"

UCLASS()
class PROJECTHALF_API AReadableNote : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AReadableNote();

	void OpenNote();
	void CloseNote();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	APawn* player;
	FVector PlayerPos;

	bool IsOpened();

private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* root;

	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleDefaultsOnly)
		UStaticMeshComponent* NoteMesh;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> NoteClass;

	UPROPERTY()
		class UUserWidget* Note;
	bool bIsOpened = false;
	UFUNCTION()
		void ShowPickUpMessage(UPrimitiveComponent* TouchedComponent);
	UFUNCTION()
		void HidePickUpMessage();
	class AProjectHALFPlayerController* PlayerController;
	/* Handle to manage the timer */
	FTimerHandle TimerHandle;

};
