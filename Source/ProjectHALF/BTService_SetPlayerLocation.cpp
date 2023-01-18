// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SetPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"

UBTService_SetPlayerLocation::UBTService_SetPlayerLocation()
{
	NodeName = "SetPlayerLocationIfSeen";
}

void UBTService_SetPlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn == nullptr) { return; }
	if (OwnerComp.GetAIOwner() == nullptr) { return; }

	if (OwnerComp.GetAIOwner()->LineOfSightTo(PlayerPawn))
	{

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
	}
	else 
	{

		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

	}
}