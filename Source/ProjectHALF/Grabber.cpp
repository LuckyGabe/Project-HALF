#include "Grabber.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "DrawDebugHelpers.h"

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle())
	{
		if (PhysicsHandle->GrabbedComponent)
		{
			FVector Start = GetComponentLocation();
			FVector End = Start + GetForwardVector() * grabDistance;

			PhysicsHandle->SetTargetLocationAndRotation(End, GetComponentRotation());
		}
	}
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	
		FHitResult HitResult;

		if (GetGrabbableInReach(HitResult))
		{
			//enable rigidbodies in grabbed object
			HitResult.GetComponent()->WakeAllRigidBodies();

			PhysicsHandle->GrabComponentAtLocationWithRotation(HitResult.GetComponent(), NAME_None, HitResult.ImpactPoint, GetComponentRotation());
		}
	
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * grabDistance;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(grabRadius);

	return GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel2, Sphere);
}

void UGrabber::Release()
{
	if (UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle())
	{
		if (PhysicsHandle->GrabbedComponent)
		{
			PhysicsHandle->ReleaseComponent();
		}
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	return GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
}
