// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerBase.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DoozyAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class LEZIONIUDEMY2_API UDoozyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	// Funzione che corrisponde al primo NODE dell'EVENT GRAPH dell'animation blueprint (Event Blueprint Update Animation)
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	AActor* Actor;
	APlayerBase* playerBase;

	UPROPERTY(BlueprintReadOnly, Category = "Conditions")
		float Speed;   // il nome che è stato dato nel blend space

	UPROPERTY(BlueprintReadOnly, Category = "Conditions")
		float Crouch;

	UPROPERTY(BlueprintReadOnly, Category = "Conditions")
		bool isInAir;

	UPROPERTY(BlueprintReadOnly, Category = "Conditions")
		bool isDead;
};
