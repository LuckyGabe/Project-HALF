// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "DrawDebugHelpers.h"
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
	//the start and end (lenght) of a vector for grabbing 
	FVector startVector = GetComponentLocation();
	FVector endVector = startVector + GetForwardVector() * grabDistance;

	physicsHandle->SetTargetLocationAndRotation(endVector, GetComponentRotation());

}
void UGrabber::Grab()
{
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
	FHitResult hitResult;

	if (GetGrabbableInReach(hitResult)) {
		//enable rigidbodies in grabbed object
		hitResult.GetComponent()->WakeAllRigidBodies();
		//grab
		physicsHandle->GrabComponentAtLocationWithRotation(hitResult.GetComponent(), NAME_None, hitResult.ImpactPoint, GetComponentRotation());
	}
}
//sphere trace objects ahead
bool UGrabber::GetGrabbableInReach(FHitResult& outHitResult) const
{
	FVector startVector = GetComponentLocation();
	FVector endVector = startVector + GetForwardVector() * grabDistance;
	FCollisionShape sphere = FCollisionShape::MakeSphere(grabRadius);
	return GetWorld()->SweepSingleByChannel(outHitResult, startVector, endVector, FQuat::Identity, ECC_GameTraceChannel2, sphere);;
}
void UGrabber::Release()
{
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
	if (physicsHandle->GetGrabbedComponent() != nullptr) 
	{
		physicsHandle->ReleaseComponent();
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	return GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
}