// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void UMyGameInstance::SavePlayerData()
{
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Player->SavePlayerData(Health, Ammo, MagAmmo, MedKitsNumb, bHasGun);

}
void UMyGameInstance::LoadPlayerData()
{
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Player->LoadPlayerData(Health, Ammo, MagAmmo, MedKitsNumb, bHasGun);

}
void UMyGameInstance::ClearPlayerData()
{
	Ammo = 0;
	MagAmmo = 0;
	MedKitsNumb = 0;
	bHasGun = 0;
	Health = 100;

}