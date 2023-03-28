#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AGun::AGun()
{
    PrimaryActorTick.bCanEverTick = false;
    root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    SetRootComponent(root);
    GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
    GunMesh->SetupAttachment(root);
}

bool AGun::GunTrace(FHitResult& OutHit, FVector& ShotDirection)
{
    AController* ownerController = GetOwnerController();
    if (ownerController == nullptr) { return false; }

    FVector location;
    FRotator rotation;
    ownerController->GetPlayerViewPoint(location, rotation);
    FVector endPoint = location + rotation.Vector() * bulletRange;

    FCollisionQueryParams params;
    params.AddIgnoredActor(this);
    params.AddIgnoredActor(GetOwner());
    ShotDirection = -rotation.Vector();

    return GetWorld()->LineTraceSingleByChannel(OutHit, location, endPoint, ECollisionChannel::ECC_GameTraceChannel3, params);
}

AController* AGun::GetOwnerController() const
{
    APawn* ownerPawn = Cast<APawn>(GetOwner());
    if (ownerPawn == nullptr) { return nullptr; }
    return ownerPawn->GetController();
}

void AGun::PullTrigger()
{
    if (MuzzleFlash)
    {
        UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GunMesh, FName("MuzzleFlashSocket"));
    }
    if (MuzzleSound)
    {
        UGameplayStatics::SpawnSoundAttached(MuzzleSound, GunMesh, TEXT("MuzzleFlashSocket"));
    }

    FVector shotDirection;
    FHitResult hitResult;
    bool bSuccess = GunTrace(hitResult, shotDirection);

    if (bSuccess)
    {
        if (ImpactEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, hitResult.Location, shotDirection.Rotation(), true);
        }
        if (ImpactSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, hitResult.Location);
        }
        if (hitResult.Actor->ActorHasTag("Glass") && GlassBreakingSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), GlassBreakingSound, hitResult.Location);
        }
        FPointDamageEvent DamageEvent(damage, hitResult, shotDirection, nullptr);
        AActor* hitActor = hitResult.GetActor();
        if (hitActor)
        {
            hitActor->TakeDamage(damage, DamageEvent, GetOwnerController(), this);
        }
    }
}
