// Fill out your copyright notice in the Description page of Project Settings.


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
#include "Keypad.h"
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
	//only shoot if player has a gun and ammunition in the magazine
	if (Gun && Gun->bGunActive && !bIsReloading && MagAmmo > 0)
	{
		MagAmmo--;

		//call gun's function
		Gun->PullTrigger();
	}
}

bool APlayerCharacter::RayTrace(FHitResult& OutHit, FVector& ShotDirection)
{

	FVector location;										  //
	FRotator rotation;										 // Get the location and rotation of player viewpoint
	GetController()->GetPlayerViewPoint(location, rotation);//

	FVector endPoint = location + rotation.Vector() * RayTraceRange; //the end point of the ray
	FCollisionQueryParams params;
	params.AddIgnoredActor(this); // ignore the player's collider
	params.AddIgnoredActor(GetOwner()); // ignore the player's collider
	ShotDirection = -rotation.Vector(); // shot the ray where the player's looking

	//return the ray trace results
	return  GetWorld()->LineTraceSingleByChannel(OutHit, location, endPoint, ECollisionChannel::ECC_GameTraceChannel1, params);
}

void APlayerCharacter::Interact()
{
	//store trace results
	FVector shotDirection;
	FHitResult hitResult;
	bool bSuccess = RayTrace(hitResult, shotDirection);

	if (bSuccess)
	{

		AActor* hitActor = hitResult.GetActor();

		//If picking up a gun
		if (hitActor != nullptr && hitActor->GetRootComponent()->ComponentHasTag(FName("Gun"))) // if the object is a gun
		{
			APickable* pickable = Cast<APickable>(hitActor); //cast the pickable script
			if (pickable && pickable->GetActive()) // make sure it's not a null pointer and the item is active
			{
				bHasGun = true;
				pickable->PickedUp(); //pick up the gun

				SpawnGun();

				//set ammunition
				MagAmmo = 16;
				Ammo = 16;
			}
		}

		//If picking up ammunition
		else if (hitActor != nullptr && hitActor->ActorHasTag("Ammo")) // if the object is an ammunition
		{
			if (Cast<APickable>(hitActor)) // prevent crashing
			{
				APickable* pickable = Cast<APickable>(hitActor); //cast the pickable script
				if (pickable->GetActive())
				{
					pickable->PickedUp(); //pick up the gun

					Ammo += 16; //add ammunition
				}
			}


		}

		//If picking up Health kit
		else if (hitActor != nullptr && hitActor->ActorHasTag("Medkit")) // if the object is a health kit
		{
			if (Cast<APickable>(hitActor)) // prevent crashing
			{
				APickable* pickable = Cast<APickable>(hitActor); //cast the pickable script
				if (pickable->GetActive())
				{
					pickable->PickedUp(); //pick up the health kit

					MedKitsNumb++; //add medkit
				}
			}
			else { UE_LOG(LogTemp, Warning, TEXT("Failed to cast ammo in Interact() function")); }

		}

		else if (hitActor != nullptr && hitActor->ActorHasTag("Note")) // if the object is a note pad
		{

			AReadableNote* note = Cast<AReadableNote>(hitActor);

			if (note && !note->IsOpened())
			{

				note->OpenNote(); // open note

				//set variables for the note
				note->PlayerPos = GetActorLocation();
				note->player = Cast<APawn>(this);
			}
		}

		else if (hitActor != nullptr && hitActor->ActorHasTag("Button")) // if the object is a button
		{

			AElevatorButton* button = Cast<AElevatorButton>(hitActor);
			if (button && !button->bIsPressed)
			{
				button->Press();

			}
		}


		//else if (hitActor != nullptr && hitActor->GetRootComponent()->ComponentHasTag(FName("Keypad"))) 
		//{

		//	AKeypad* keypad = Cast<AKeypad>(hitActor);
		//	if (keypad && !keypad->IsOpened())
		//	{
		//		keypad->OpenKeypad();
		//		keypad->PlayerPos = GetActorLocation();
		//		keypad->player = Cast<APawn>(this);
		//	}
		//}


	}

}

//override TakeDamage function
float APlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageApplied = FMath::Min(Health, DamageApplied); //Damage applied can't be higher value than health

	Health -= DamageApplied; //Decrease health

	if (PlayerHurtSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), PlayerHurtSound);
	}


	if (Health <= 0)
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
	return DamageApplied;

}


bool APlayerCharacter::IsReloading() const
{

	return bIsReloading;

}

void APlayerCharacter::Reload()
{
	//if player has a gun and ammunition
	if (Gun && MagAmmo < 16 && Ammo>0)
	{
		bIsReloading = true;

		float amount = 16 - MagAmmo; //calculate the amount of bullets that will be moved to magazine

		for (int i = 0; i < amount; i++)
		{
			if (Ammo > 0)
			{
				Ammo--;
				MagAmmo++;

				if (Ammo < 0) { Ammo = 0; }
			}


		}

		//reset reloading boolean
		GetWorldTimerManager().SetTimer(ReloadingHandle, this, &APlayerCharacter::ResetReload, ReloadTime, false);

	}


}

void APlayerCharacter::ResetReload()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReloadingSound, Gun->GetActorLocation());
	bIsReloading = false;

}

float APlayerCharacter::GetHealthPercent() const
{
	return (Health / MaxHealth);
}

float APlayerCharacter::GetMagAmmunition() const
{
	return MagAmmo;
}

float APlayerCharacter::GetAmmunition() const
{
	return Ammo;
}

int APlayerCharacter::GetMedKitsNumb() const
{
	return MedKitsNumb;
}

FString APlayerCharacter::GetHealthText() const
{
	FString HealthPercText = FString::SanitizeFloat(GetHealthPercent() * 100.f);
	FString text = "Health: " + HealthPercText + "%";

	return text;
}

void APlayerCharacter::SavePlayerData(float& OutHealth, float& OutAmmo, float& OutMagAmmo, int& OutMedKitsNumb, bool& bPlayerHasGun)
{
	OutHealth = this->Health;
	OutAmmo = this->Ammo;
	OutMagAmmo = this->MagAmmo;
	OutMedKitsNumb = this->MedKitsNumb;
	bPlayerHasGun = this->bHasGun;
}

void APlayerCharacter::LoadPlayerData(float NewHealth, float NewAmmo, float NewMagAmmo, int NewMedKitsNumb, bool bPlayerHasGun)
{
	this->Health = NewHealth;
	this->Ammo = NewAmmo;
	this->MagAmmo = NewMagAmmo;
	this->bHasGun = bPlayerHasGun;
	this->MedKitsNumb = NewMedKitsNumb;
}

void APlayerCharacter::SpawnGun()
{
	//spawn gun
	Gun = GetWorld()->SpawnActor<AGun>(GunBP);

	Gun->bGunActive = true;

	//Attach to player's hand
	Gun->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("GunSocket"));
	//set the player as a owner of the gun
	Gun->SetOwner(this);

}

void APlayerCharacter::Heal()
{

	if (MedKitsNumb > 0)
	{
		MedKitsNumb--;
		Health += 40;

	}

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