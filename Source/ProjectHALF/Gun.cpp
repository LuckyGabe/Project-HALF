// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(root);
	bGunActive = false;
	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult& OutHit, FVector& ShotDirection)
{
	//the location and rotation of the start point of the ray
	FVector location;
	FRotator rotation;

	AController* ownerController = GetOwnerController();

	if (ownerController == nullptr) { return false; }

	//get owner's viewpoint
	ownerController->GetPlayerViewPoint(location, rotation);

	FVector endPoint = location + rotation.Vector() * bulletRange; //calculate the end point of the trace
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(GetOwner());

	ShotDirection = -rotation.Vector();
	
	return  GetWorld()->LineTraceSingleByChannel(OutHit, location, endPoint, ECollisionChannel::ECC_GameTraceChannel3, params);
}


AController* AGun::GetOwnerController() const
{
	APawn* ownerPawn = Cast<APawn>(GetOwner());
	if (ownerPawn == nullptr) { return nullptr; }
	return ownerPawn->GetController();

}

void AGun::PullTrigger()
{
	if (MuzzleFlash != nullptr) 
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GunMesh, FName("MuzzleFlashSocket"));
	}

	FVector shotDirection; //store shot direction
	AController* ownerController = GetOwnerController();
	FHitResult hitResult;
	bool bSuccess = GunTrace(hitResult, shotDirection);

	if (bSuccess)
	{
		if(ImpactEffect!= nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, hitResult.Location, shotDirection.Rotation(), true);
		}

		FPointDamageEvent DamageEvent(damage, hitResult, shotDirection, nullptr);
		AActor* hitActor = hitResult.GetActor();
		if (hitActor != nullptr)
		{

			hitActor->TakeDamage(damage, DamageEvent, ownerController, this);

		}

	}

}