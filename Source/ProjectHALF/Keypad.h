// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Keypad.generated.h"

UCLASS()
class PROJECTHALF_API AKeypad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeypad();
	APawn* player;
	FVector PlayerPos;
	void OpenKeypad();
	void CloseKeypad();
	bool IsOpened();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* root;

	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleDefaultsOnly)
		UStaticMeshComponent* KeypadMesh;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> KeypadClass;

	UPROPERTY()
		class UUserWidget* Keypad;
	bool bIsOpened = false;

};
