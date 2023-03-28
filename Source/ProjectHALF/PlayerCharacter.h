#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

class UCameraComponent;
class USkeletalMeshComponent;
class AGun;

#include "PlayerCharacter.generated.h"

UCLASS()
class PROJECTHALF_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//Saving & Loading player's data
	UFUNCTION(BlueprintCallable)
		void SavePlayerData(float& OutHealth, float& OutAmmo, float& OutMagAmmo, int& OutMedKitsNumb, bool& bPlayerHasGun);

	UFUNCTION(BlueprintCallable)
		void LoadPlayerData(float NewHealth, float NewAmmo, float NewMagAmmo, int NewMedKitsNumb, bool bPlayerHasGun);

	void StartCrouch();
	void StopCrouch();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void Shoot();
	void Reload();
	void Interact();
	void Heal();
	UFUNCTION(BlueprintCallable)
	void PauseGame();


	UPROPERTY(BlueprintReadWrite)
		bool bHasGun = false;
	UFUNCTION(BlueprintPure)
		bool IsReloading() const;
	UFUNCTION(BlueprintPure)
		float GetHealthPercent() const;
	UFUNCTION(BlueprintPure)
		float GetMagAmmunition() const;
	UFUNCTION(BlueprintPure)
		float GetAmmunition() const;
	UFUNCTION(BlueprintPure)
		int32 GetMedKitsNumb() const;
	UFUNCTION(BlueprintPure)
		FString GetHealthText() const;
	UFUNCTION(BlueprintPure)
	float GetHealth() const;
	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const;
	void SpawnGun();
private:
	void MoveForward(float scale);
	void MoveRight(float scale);
	void Turn(float scale);
	void LookUp(float scale);
	bool RayTrace(FHitResult& OutHit, FVector& ShotDirection);
	void ResetReload();
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, Category = "Health")
		float Health;
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		int32 medKitsNumb;
	UPROPERTY(EditDefaultsOnly, Category = "Gun")
		TSubclassOf<AGun> GunBP;

	UPROPERTY()
		AGun* Gun;
	FTimerHandle ReloadingHandle;
	UPROPERTY(EditDefaultsOnly, Category = "Ray Trace")
		float RayTraceRange = 1000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float TurnRate = 45.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float LookUpRate = 45.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
		int32 MaxAmmo = 20;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
		int32 MagAmmo = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
		int32 Ammo = 20;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactable", meta = (AllowPrivateAccess = "true"))
		AActor* CurrentInteractable;
protected:
	UPROPERTY(EditDefaultsOnly)
		float reloadTime = 2;

	UPROPERTY(EditAnywhere)
		USoundBase* PlayerHurtSound;

	UPROPERTY(EditAnywhere)
		USoundBase* ReloadingSound;

	UPROPERTY(EditAnywhere)
		USoundBase* DeathSound;
	UPROPERTY()
	bool bIsReloading = false;


};
