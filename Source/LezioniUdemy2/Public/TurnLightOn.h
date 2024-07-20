// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "Components/LightComponent.h" 
#include "Components/BoxComponent.h" 
#include "TurnLightOn.generated.h"

/**
 * 
 */
UCLASS()
class LEZIONIUDEMY2_API ATurnLightOn : public AInteractionActor
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TArray<ULightComponent*> Lights;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollision;

	virtual void Interact() override;
};
