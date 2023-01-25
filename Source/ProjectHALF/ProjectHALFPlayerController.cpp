// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectHALFPlayerController.h"
#include <Blueprint/UserWidget.h>


AProjectHALFPlayerController::AProjectHALFPlayerController()
{
	
}

	// Called when the game starts or when spawned
void AProjectHALFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDClass)
	{
		PlayerHUD = CreateWidget<UUserWidget>(this, HUDClass);
		PlayerHUD->AddToViewport();
	}
	
	
	
}
