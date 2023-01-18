// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
// Called when the game starts or when spawned
void AMyAIController::BeginPlay()
{
    Super::BeginPlay();
    if (AIBehaviorTree != nullptr)
    {
       /* RunBehaviorTree(AIBehaviorTree);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());*/
        
    }
}

// Called every frame
void AMyAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
    MoveToActor(Player, 400.0f);
   
}