// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "EnemyMonster.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


AEnemyController::AEnemyController()
{
	blackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	check(blackboardComponent);

	beaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	check(beaviorTreeComponent);
}

void AEnemyController::OnPossess(APawn* inPawn)
{
	Super::OnPossess(inPawn);

	if (inPawn == nullptr)
	{
		return;
	}

	AEnemyMonster* enemy = Cast<AEnemyMonster>(inPawn);
	if (enemy)
	{
		if (enemy->GetBehaviorTree())
		{
			blackboardComponent->InitializeBlackboard(*(enemy->GetBehaviorTree()->BlackboardAsset));
		}
	}
}
