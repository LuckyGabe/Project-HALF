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

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SkeletalMesh->SetupAttachment(CameraComponent);
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
	else { UE_LOG(LogTemp, Error, TEXT("Failed to get player controller")); }
	Health = MaxHealth;
	
}


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	Crouch();
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

void APlayerCharacter::StopCrouch()
{
	UnCrouch();
}
void APlayerCharacter::Shoot()
{
	if (Gun && Gun->bGunActive)
	{
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
	DrawDebugPoint(GetWorld(), endPoint, 40.f, FColor::Red, true, 50.f);
	
	//return the ray trace results
	return  GetWorld()->LineTraceSingleByChannel(OutHit, location, endPoint, ECollisionChannel::ECC_GameTraceChannel1, params);
}

void APlayerCharacter::Interact()
{
	//store trace results
	FVector shotDirection;
	FHitResult hitResult;
	UE_LOG(LogTemp, Warning, TEXT("Interact"));
	bool bSuccess = RayTrace(hitResult, shotDirection);

	if (bSuccess)
	{
		
		AActor* hitActor = hitResult.GetActor();
	
		if (hitActor != nullptr && hitActor->GetRootComponent()->ComponentHasTag(FName("Gun"))) // if the object is a gun
		{
			APickable* pickable = Cast<APickable>(hitActor); //cast the pickable script
			if (pickable && pickable->GetActive()) // make sure it's not a null pointer and the item is active
			{
				pickable->PickedUp(); //pick up the gun

				Gun = GetWorld()->SpawnActor<AGun>(GunBP);
				Gun->bGunActive = true;
				Gun->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("GunSocket"));
				Gun->SetOwner(this);
				
			}
			else { UE_LOG(LogTemp, Warning, TEXT("Failed to cast gun in Interact() function")); }
		}
		else if (hitActor != nullptr && hitActor->GetRootComponent()->ComponentHasTag(FName("Ammo"))) // if the object is an ammunition
		{
			if (Cast<APickable>(hitActor)) // prevent crashing
			{
				APickable* pickable = Cast<APickable>(hitActor); //cast the pickable script
				if (pickable->GetActive())
				{
					pickable->PickedUp(); //pick up the gun

					/*
					ADD AMMO

					*/
				}
			}
			else { UE_LOG(LogTemp, Warning, TEXT("Failed to cast ammo in Interact() function")); }

		}

	}

}

float APlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageApplied = FMath::Min(Health, DamageApplied);
	Health -= DamageApplied;

	UE_LOG(LogTemp, Warning, TEXT("Health left: %f"), Health);
	return DamageApplied;

}