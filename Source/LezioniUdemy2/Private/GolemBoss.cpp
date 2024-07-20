// Fill out your copyright notice in the Description page of Project Settings.


#include "GolemBoss.h"

// Sets default values
AGolemBoss::AGolemBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGolemBoss::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGolemBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGolemBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

