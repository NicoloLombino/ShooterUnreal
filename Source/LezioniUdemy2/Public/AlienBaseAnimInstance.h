// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemyMonster.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AlienBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LEZIONIUDEMY2_API UAlienBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void UpadateAnimationProperties(float deltaTime);

protected:
	// Funzione che corrisponde al primo NODE dell'EVENT GRAPH dell'animation blueprint (Event Blueprint Update Animation)
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	AActor* Actor;
	AEnemyMonster* enemyMonster;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Conditions", meta = (AllowPrivateAccess = "true"))
		float Speed;   // il nome che è stato dato nel blend space

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Conditions", meta = (AllowPrivateAccess = "true"))
		bool isAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Conditions", meta = (AllowPrivateAccess = "true"))
		bool isDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Conditions", meta = (AllowPrivateAccess = "true"))
		bool isScreaming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class AEnemyMonster* enemy;

};
