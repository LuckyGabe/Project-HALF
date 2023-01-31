// Fill out your copyright notice in the Description page of Project Settings.


#include "ElevatorButton.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AElevatorButton::AElevatorButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = ButtonMesh;
}

// Called when the game starts or when spawned
void AElevatorButton::BeginPlay()
{
	Super::BeginPlay();
	OriginalLocation = GetActorLocation();
	
}

// Called every frame
void AElevatorButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

	FVector CurrentPosition = GetActorLocation();
	FVector TargetPosition = OriginalLocation + MoveOffSet;
	float MoveSpeed = FVector::Dist(OriginalLocation, TargetPosition) / MoveTime;
	FVector NewLocation = FMath::VInterpConstantTo(CurrentPosition, TargetPosition, DeltaTime, MoveSpeed);

	if (bIsPressed)
	{
		SetActorLocation(NewLocation);

	}


}

void AElevatorButton::Press()
{
	
	if(bHasPower)
	{
	bIsPressed = true;
		// load next level
	UGameplayStatics::OpenLevel(this, LevelToOpen);

	UE_LOG(LogTemp, Warning, TEXT("Button Pressed"));
	}

	
}
