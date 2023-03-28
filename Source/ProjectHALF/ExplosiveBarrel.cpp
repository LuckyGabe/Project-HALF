#include "ExplosiveBarrel.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AExplosiveBarrel::AExplosiveBarrel()
{
	PrimaryActorTick.bCanEverTick = false;

	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	BarrelMesh->SetupAttachment(RootComponent);
}

float AExplosiveBarrel::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageApplied = FMath::Min(Health, DamageApplied);
	Health -= DamageApplied;
	if (Health <= 0) { Explode(); }
	return DamageApplied;
}

void AExplosiveBarrel::Explode()
{
	if (bExploded) { return; }
	bExploded = true;

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	}

	//the start and end location of trace can be the same since the radius of the sphere trace is used
	const FVector Start = GetActorLocation();
	const FVector End = GetActorLocation();

	TArray<AActor*> IgnoredActors = { this };
	TArray<FHitResult> HitResultArray;

	if (Explosion != nullptr)
	{
		//spawn explosion (particle effect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, GetActorLocation());
	}

	//sphere trace to get all the actors around
	bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(), Start, End, ExplosionRadius,
		UEngineTypes::ConvertToTraceType(ECC_Camera), false, IgnoredActors, EDrawDebugTrace::None,
		HitResultArray, true);

	if (bHit)
	{
		for (auto& HitResult : HitResultArray)
		{
			//store found actor
			AActor* ActorInRange = HitResult.GetActor();
			if (ActorInRange != nullptr)
			{
				//deal damage to the actor
				FPointDamageEvent DamageEvent(Damage, HitResult, GetActorLocation(), nullptr);
				ActorInRange->TakeDamage(Damage, DamageEvent, NULL, this);
			}
		}
	}

	SetLifeSpan(0.2f); //destroy this object after 0.2s
}
