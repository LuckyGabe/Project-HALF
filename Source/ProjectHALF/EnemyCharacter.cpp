// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Gun.h"
// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent"); // create a camera
	CameraComponent->SetupAttachment(RootComponent); //attach camera to the root component
	CameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	health = maxHealth;
	if (gunBP != nullptr)
	{
		gun = GetWorld()->SpawnActor<AGun>(gunBP);
		GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
		gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GunSocket"));
		gun->SetOwner(this);
		
	}
	else { UE_LOG(LogTemp, Error, TEXT("Failed to get gun for the enemy")); }
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (health <= 0)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
	}
}





// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void AEnemyCharacter::Shoot()
{
	gun->PullTrigger();
}
bool AEnemyCharacter::IsDead() const
{
	if (health <= 0) {   return true; }
	else return false;
}

float AEnemyCharacter::GetHealthPercentage() const
{
	return health / maxHealth;
}

float AEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float damageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	damageApplied = FMath::Min(health, damageApplied);
	health -= damageApplied;
	if (IsDead())
	{
		DetachFromControllerPendingDestroy();
	}
	return damageApplied;

}