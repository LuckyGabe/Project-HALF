
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

private:
	void MoveForward(float scale); //Move forward/backward
	void MoveRight(float scale); // Move left/right
	void Turn(float scale);
	void LookUp(float scale);
	void StartCrouch();
	void StopCrouch();
	bool RayTrace(FHitResult& OutHit, FVector& ShotDirection); // bool if raytracing was succesfull and return the results
	void Interact(); //interact with items
	void Shoot();
	void Reload();
	void ResetReload();
	void Heal(); //heal the player 

	int MedKitsNumb; //number of healing items

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//Saving & Loading player's data
	UFUNCTION(BlueprintCallable)
		void SavePlayerData(float& OutHealth, float& OutAmmo, float& OutMagAmmo, int& OutMedKitsNumb, bool& bPlayerHasGun);

	UFUNCTION(BlueprintCallable)
		void LoadPlayerData(float NewHealth, float NewAmmo, float NewMagAmmo, int NewMedKitsNumb, bool bPlayerHasGun);


	UFUNCTION()
		void SpawnGun();
	//These function are for the UI and blueprint callable
	UFUNCTION(BlueprintPure)
		bool IsReloading() const;

	UFUNCTION(BlueprintPure)
		float GetHealthPercent() const;

	UFUNCTION(BlueprintPure)
		FString GetHealthText() const;

	UPROPERTY(BlueprintReadWrite)
		bool bHasGun = false;

	UFUNCTION(BlueprintPure)
		float GetMagAmmunition() const;

	UFUNCTION(BlueprintPure)
		float GetAmmunition() const;

	UFUNCTION(BlueprintPure)
		int GetMedKitsNumb() const;

	UFUNCTION(BlueprintCallable)
		void PauseGame();

	//Protected variables & components
protected:
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float TurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float LookUpRate;

	//How far should the race trace for interaction reach
	UPROPERTY(EditAnywhere, Category = "Interaction")
		float RayTraceRange;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf< AGun> GunBP;
	UPROPERTY()
		AGun* Gun;

	UPROPERTY(EditDefaultsOnly)
		float MaxHealth = 100;
	UPROPERTY(EditDefaultsOnly)
		float ReloadTime = 2;
	UPROPERTY(VisibleAnywhere)
		float Health;
	// ammunition in the magazine
	UPROPERTY(VisibleAnywhere)
		float MagAmmo = 0;
	//how much ammunition the player has
	UPROPERTY(VisibleAnywhere)
		float Ammo = 0;

	UPROPERTY()
		bool bIsReloading = false;

	FTimerHandle ReloadingHandle;

	UPROPERTY(EditAnywhere)
		USoundBase* PlayerHurtSound;

	UPROPERTY(EditAnywhere)
		USoundBase* ReloadingSound;

	UPROPERTY(EditAnywhere)
		USoundBase* DeathSound;
};
