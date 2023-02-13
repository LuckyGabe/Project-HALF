// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
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
	if (!bExploded)
	{
		if (ExplosionSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
		}
		bExploded = true;
		//the start and end location of trace can be the same since the radius of the sphere trace is used
		const FVector Start = GetActorLocation();
		const FVector End = GetActorLocation();
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(this); //add this object to the ignored actors
		TArray<FHitResult> HitResultArray; //store hit actors
		if (Explosion != nullptr)
		{
			//spawn explosion (particle effect)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, GetActorLocation());
		}
		//sphere trace to get all the actors around
		bool Hit = UKismetSystemLibrary::SphereTraceMulti(
			GetWorld(), Start, End, ExplosionRadius,
			UEngineTypes::ConvertToTraceType(ECC_Camera), false, IgnoredActors, EDrawDebugTrace::None,
			HitResultArray, true);
		if (Hit)
		{
			for (int i = 0; i < HitResultArray.Num(); i++) //iterate through all found actors
			{
				FPointDamageEvent DamageEvent(Damage, HitResultArray[i], GetActorLocation(), nullptr);
				//store found actor
				AActor* ActorInRange = HitResultArray[i].GetActor();
				if (ActorInRange != nullptr)
				{
					//deal damage to the actor
					ActorInRange->TakeDamage(Damage, DamageEvent, NULL, this);
				}
			}
		}
		SetLifeSpan(0.2f); //destroy this object after 0.2s
	}
}