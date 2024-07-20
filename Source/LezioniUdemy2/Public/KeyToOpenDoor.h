// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.h"
#include "KeyToOpenDoor.generated.h"

UCLASS()
class LEZIONIUDEMY2_API AKeyToOpenDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeyToOpenDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	ADoor* DoorToOpen;

	UFUNCTION(BlueprintCallable)
	void OpenTheDoor();
};
