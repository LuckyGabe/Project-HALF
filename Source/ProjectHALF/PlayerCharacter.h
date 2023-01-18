// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class AGun;

UCLASS()
class PROJECTHALF_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

//Protected Functions
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void MoveForward(float scale); //Move forward/backward
	void MoveRight(float scale); // Move left/right
	void Turn(float scale);
	void LookUp(float scale);
	void StartCrouch();
	void StopCrouch();
	bool RayTrace(FHitResult& OutHit, FVector& ShotDirection); // bool if raytracing was succesfull and return the results
	void Interact();
	void Shoot();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

//Protected variables & components
protected:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleDefaultsOnly)
		USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float TurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float LookUpRate;
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float RayTraceRange;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf< AGun> GunBP;

	UPROPERTY()
		AGun* Gun;
	UPROPERTY(EditDefaultsOnly)
		float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
		float Health;


//Public variables & components
public:

	
	
	
};
