// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class LEZIONIUDEMY2_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	

public:

	AEnemyController();
	virtual void OnPossess(APawn* inPawn) override;

private:

	// components for enemy Blackboard
	UPROPERTY(BlueprintReadWrite, Category = "AI Behaviour", meta = (AllowPrivateAccess = "true"))
		class UBlackboardComponent* blackboardComponent;

	// components for enemy Behaviour
	UPROPERTY(BlueprintReadWrite, Category = "AI Behaviour", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* beaviorTreeComponent;

public:
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return blackboardComponent; }
};
