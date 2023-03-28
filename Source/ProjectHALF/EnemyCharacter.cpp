#include "EnemyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Components/CapsuleComponent.h"
#include "DoorMover.h"
#include "Gun.h"

AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(GetRootComponent());
	CameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
	CameraComponent->bUsePawnControlRotation = true;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	health = maxHealth;

	if (gunBP)
	{
		gun = GetWorld()->SpawnActor<AGun>(gunBP);
		auto mesh = GetMesh();
		check(mesh);
		mesh->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
		gun->AttachToComponent(mesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("GunSocket"));
		gun->SetOwner(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get gun for the enemy"));
	}
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (health <= 0.f)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
	}
}

void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacter::Shoot()
{
	if (gun)
	{
		gun->PullTrigger();
	}
}

bool AEnemyCharacter::IsDead() const
{
	return health <= 0.f;
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
		if (IsBoss)
		{
			OpenGate();
			IsBoss = false;
		}
		DetachFromControllerPendingDestroy();
	}

	return damageApplied;
}

void AEnemyCharacter::OpenGate()
{
	const FName doorTag{ TEXT("Door") };
	TArray<AActor*> doors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), doorTag, doors);

	for (AActor* actor : doors)
	{
		if (actor)
		{
			UDoorMover* door = Cast<UDoorMover>(actor->GetComponentByClass(UDoorMover::StaticClass()));
			if (door)
			{
				door->bLockedOpen = true;
				door->bShouldMove = true;
			}
		}
	}
}
