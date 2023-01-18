// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"
class AGun;
class UCameraComponent;
UCLASS()
class PROJECTHALF_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UFUNCTION(BlueprintPure)
		bool IsDead() const;

	UFUNCTION(BlueprintPure)
		float GetHealthPercentage() const;

	void Shoot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveForward(float scale); //Move forward/backward
	void MoveRight(float scale); // Move left/right
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf< AGun> gunBP;

	UPROPERTY(EditDefaultsOnly)
		float maxHealth = 100;

	UPROPERTY(VisibleAnywhere)
		float health;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* CameraComponent;


	UPROPERTY()
		AGun* gun;
};
