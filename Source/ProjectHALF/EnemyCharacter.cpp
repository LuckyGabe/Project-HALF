// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Components/CapsuleComponent.h"
#include "DoorMover.h"
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
		gun = GetWorld()->SpawnActor<AGun>(gunBP); //spawn gun

		GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None); //hide bone (It's a Wraith's gun mesh)
		//attach newly created gun to the socket
		gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GunSocket"));
		gun->SetOwner(this); //set gun's owner

	}
	else { UE_LOG(LogTemp, Error, TEXT("Failed to get gun for the enemy")); }
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (health <= 0)
	{
		//if this enemy is dead disable collisions and detach AI controller
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

//Function to be passed to animation blueprint
bool AEnemyCharacter::IsDead() const
{
	if (health <= 0) { return true; }
	else return false;
}

float AEnemyCharacter::GetHealthPercentage() const
{
	return health / maxHealth;
}

//override take damage
float AEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float damageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	damageApplied = FMath::Min(health, damageApplied); //damage applied can't be larger than health
	health -= damageApplied;
	if (IsDead())
	{
		if (IsBoss)
		{
			OpenGate();
			IsBoss = false;
		}
		DetachFromControllerPendingDestroy();
		

	}
	return damageApplied;
}

void AEnemyCharacter::OpenGate() //Opening gate at level 3
{
	FName doorTag = "Door";
	TArray<AActor*> doors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), doorTag, doors);
	
		for(int i = 0; i < doors.Num(); i++)
		{
			if (doors[i])
			{
				UDoorMover* door = Cast<UDoorMover>(doors[i]->FindComponentByClass<UDoorMover>());
				if (door)
				{
					door->bLockedOpen = true;
					door->bShouldMove = true;
				}
			}
		}
	
}