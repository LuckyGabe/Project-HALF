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


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
	FVector startVector = GetComponentLocation();
	FVector endVector = startVector + GetForwardVector() * grabDistance;
	
		physicsHandle->SetTargetLocationAndRotation(endVector, GetComponentRotation());
	
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();

	FHitResult hitResult;

	if (GetGrabbableInReach(hitResult)) {

		hitResult.GetComponent()->WakeAllRigidBodies();
		FString hitActorName = hitResult.GetActor()->GetName();
		

		//grab
		physicsHandle->GrabComponentAtLocationWithRotation(hitResult.GetComponent(), NAME_None, hitResult.ImpactPoint, GetComponentRotation());


	}

}

bool UGrabber::GetGrabbableInReach(FHitResult& outHitResult) const
{
	FVector startVector = GetComponentLocation();
	FVector endVector = startVector + GetForwardVector() * grabDistance;
	FCollisionShape sphere = FCollisionShape::MakeSphere(grabRadius);

	//DrawDebugLine(GetWorld(), startVector, endVector, FColor::Red, false, 10.f);
	

	return GetWorld()->SweepSingleByChannel(outHitResult, startVector, endVector, FQuat::Identity, ECC_GameTraceChannel2, sphere);;
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
	if (physicsHandle->GetGrabbedComponent() != nullptr) {
		physicsHandle->ReleaseComponent();
	}

}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	return GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

}