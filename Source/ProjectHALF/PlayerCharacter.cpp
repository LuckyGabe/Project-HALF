
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Pickable.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Gun.h"
#include "DrawDebugHelpers.h"
#include "ReadableNote.h"
#include "ElevatorButton.h"
#include "ProjectHALFPlayerController.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = GetCapsuleComponent(); //set the capsule as a root component for the character

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent"); // create a camera
	CameraComponent->SetupAttachment(RootComponent); //attach camera to the root component
	CameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	//create fps arms
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPS_Arms"));
	SkeletalMesh->SetupAttachment(CameraComponent);

	SkeletalMesh->bCastDynamicShadow = false;
	SkeletalMesh->CastShadow = false;


}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (playerController != nullptr)
	{
		playerController->bEnableMouseOverEvents = true; // enable mouse over events
	}

	Health = MaxHealth;

}


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if the is no more ammo in the magazine, reload
	if (MagAmmo == 0 && Gun && Ammo > 0) { Reload(); }

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind movement
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &APlayerCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Released, this, &APlayerCharacter::StopCrouch);

	//Bind actions
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &APlayerCharacter::Shoot);
	PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &APlayerCharacter::Reload);
	PlayerInputComponent->BindAction("Heal", EInputEvent::IE_Pressed, this, &APlayerCharacter::Heal);
	PlayerInputComponent->BindAction("Pause", EInputEvent::IE_Pressed, this, &APlayerCharacter::PauseGame);
}

void APlayerCharacter::MoveForward(float scale)
{
	AddMovementInput(GetActorForwardVector(), scale); // Move the character forward if the scale is 1 and backwards if the scale is -1


}

void APlayerCharacter::MoveRight(float scale)
{

	AddMovementInput(GetActorRightVector(), scale); //Move actor right if the scale is 1 and left if it is -1

}

void APlayerCharacter::Turn(float scale)
{
	AddControllerYawInput(scale * TurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUp(float scale)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(scale * LookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::StartCrouch()
{

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true; //enable crouching
	Crouch();

}

void APlayerCharacter::StopCrouch()
{
	UnCrouch();
}


void APlayerCharacter::Shoot()
{
	if (Gun && Gun->bGunActive && !bIsReloading && MagAmmo > 0)
	{
		MagAmmo--;
		Gun->PullTrigger();
	}
}

bool APlayerCharacter::RayTrace(FHitResult& OutHit, FVector& ShotDirection)
{
	 FVector Location;										  //
	 FRotator Rotation;										 // Get the location and rotation of player viewpoint
	GetController()->GetPlayerViewPoint(Location, Rotation);//

	const FVector EndPoint = Location + Rotation.Vector() * RayTraceRange; //the end point of the ray

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	ShotDirection = -Rotation.Vector();

	return GetWorld()->LineTraceSingleByChannel(OutHit, Location, EndPoint, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

void APlayerCharacter::Interact()
{
	//store trace results
	FHitResult HitResult;
	FVector ShotDirection;

	if (!RayTrace(HitResult, ShotDirection)) { UE_LOG(LogTemp, Warning, TEXT("Nothing Found"));  return; }
	if (!HitResult.GetActor()) return;
	if(RayTrace(HitResult, ShotDirection)){ UE_LOG(LogTemp, Warning, TEXT("Object hit")); }


	if (HitResult.GetActor()->GetRootComponent()->ComponentHasTag(FName("Gun")))
	{
		APickable* Pickable = Cast<APickable>(HitResult.GetActor());

		if (Pickable && Pickable->GetActive())
		{
			bHasGun = true;
			Pickable->PickedUp();
			SpawnGun();
			MagAmmo = 16;
			Ammo = 20;
		}
	}
	else if (HitResult.GetActor()->ActorHasTag("Ammo"))
	{
		APickable* Pickable = Cast<APickable>(HitResult.GetActor());

		if (Pickable && Pickable->GetActive())
		{
			Pickable->PickedUp();
			Ammo += 16;
		}
	}
	else if (HitResult.GetActor()->ActorHasTag("Medkit"))
	{
		APickable* Pickable = Cast<APickable>(HitResult.GetActor());

		if (Pickable && Pickable->GetActive())
		{
			Pickable->PickedUp();
			medKitsNumb++;
		}
	}
	else if (HitResult.GetActor()->ActorHasTag("Note"))
	{
		AReadableNote* Note = Cast<AReadableNote>(HitResult.GetActor());

		if (Note && !Note->IsOpened())
		{
			Note->OpenNote();
			Note->PlayerPos = GetActorLocation();
			Note->player = Cast<APawn>(this);
		}
	}
	else if (HitResult.GetActor()->ActorHasTag("Button"))
	{
		AElevatorButton* Button = Cast<AElevatorButton>(HitResult.GetActor());

		if (Button && !Button->bIsPressed)
		{
			Button->Press();
		}
	}
}

//override TakeDamage function
float APlayerCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	const float PrevHealth = Health;
	Health = FMath::Max(0.f, Health - DamageApplied);

	if (PrevHealth > 0.f && Health <= 0.f)
	{
		AProjectHALFPlayerController* controller = Cast<AProjectHALFPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (controller)
		{
			controller->bGameOver = true;
		}
		if (DeathSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), DeathSound);
		}
	}

	if (PlayerHurtSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), PlayerHurtSound);
	}

	return DamageApplied;
}

void APlayerCharacter::Reload()
{
	if (Gun && MagAmmo < 16 && Ammo > 0)
	{
		const int32 Amount = FMath::Min(16 - MagAmmo, Ammo);
		Ammo -= Amount;
		MagAmmo += Amount;
		check(Ammo >= 0);

		bIsReloading = true;
		GetWorldTimerManager().SetTimer(ReloadingHandle, this, &APlayerCharacter::ResetReload, reloadTime, false);
	}
}

void APlayerCharacter::ResetReload()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReloadingSound, Gun->GetActorLocation());
	bIsReloading = false;
}

float APlayerCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

float APlayerCharacter::GetMagAmmunition() const
{
	return MagAmmo;
}

float APlayerCharacter::GetAmmunition() const
{
	return Ammo;
}

bool APlayerCharacter::IsReloading() const
{
	return bIsReloading;
}

int32 APlayerCharacter::GetMedKitsNumb() const
{
	return medKitsNumb;
}

FString APlayerCharacter::GetHealthText() const
{
	return FString::Printf(TEXT("Health: %.0f%%"), GetHealthPercent() * 100.f);
}

void APlayerCharacter::SpawnGun()
{
	check(GunBP);
	Gun = GetWorld()->SpawnActor<AGun>(GunBP);

	Gun->bGunActive = true;
	Gun->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("GunSocket"));
	Gun->SetOwner(this);
}
float APlayerCharacter::GetHealth()const
{
	return Health;
}

float APlayerCharacter::GetMaxHealth()const
{
	return MaxHealth;
}


void APlayerCharacter::Heal()
{
	if (medKitsNumb > 0)
	{
		medKitsNumb--;
		Health = FMath::Min(MaxHealth, Health + 40.f);
	}
}



void APlayerCharacter::SavePlayerData(float& OutHealth, float& OutAmmo, float& OutMagAmmo, int& OutMedKitsNumb, bool& bPlayerHasGun)
{
	OutHealth = this->Health;
	OutAmmo = this->Ammo;
	OutMagAmmo = this->MagAmmo;
	OutMedKitsNumb = this->medKitsNumb;
	bPlayerHasGun = this->bHasGun;
}
void APlayerCharacter::LoadPlayerData(float NewHealth, float NewAmmo, float NewMagAmmo, int NewMedKitsNumb, bool bPlayerHasGun)
{
	this->Health = NewHealth;
	this->Ammo = NewAmmo;
	this->MagAmmo = NewMagAmmo;
	this->bHasGun = bPlayerHasGun;
	this->medKitsNumb = NewMedKitsNumb;
}
void APlayerCharacter::PauseGame()
{
	AProjectHALFPlayerController* controller = Cast<AProjectHALFPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (controller)
	{
		if (controller->bGamePaused)
		{
			controller->bGamePaused = false;
		}
		else
		{
			controller->bGamePaused = true;
		}
	}
}