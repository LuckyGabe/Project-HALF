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

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
		
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	UE_LOG(LogTemp, Warning, TEXT("Explode"));

	//the start and end location of trace can be the same since the radius of the sphere trace is used
	const FVector Start = GetActorLocation();
	const FVector End = GetActorLocation();
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	TArray<FHitResult> HitResultArray;
	
	if (Explosion != nullptr) 
	{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, GetActorLocation());
	}
	
	bool Hit = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(), Start, End, ExplosionRadius, 
		UEngineTypes::ConvertToTraceType(ECC_Camera), false, IgnoredActors, EDrawDebugTrace::None, 
		HitResultArray, true);

	if (Hit)
	{
		for (int i = 0; i < HitResultArray.Num(); i++)
		{
		
			AActor* ActorInRange = HitResultArray[i].GetActor();
			if (ActorInRange != nullptr) 
			{
				ActorInRange->TakeDamage(Damage, FDamageEvent(), NULL, this);
			}
			
		}
	}
	Destroy();
}