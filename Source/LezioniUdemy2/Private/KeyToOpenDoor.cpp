// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyToOpenDoor.h"

// Sets default values
AKeyToOpenDoor::AKeyToOpenDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKeyToOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKeyToOpenDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKeyToOpenDoor::OpenTheDoor()
{
	DoorToOpen->UnlockDoor();
}

