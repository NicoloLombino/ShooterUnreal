// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienBaseAnimInstance.h"

void UAlienBaseAnimInstance::UpadateAnimationProperties(float deltaTime)
{
	if (enemy == nullptr)
	{
		enemy = Cast<AEnemyMonster>(TryGetPawnOwner());
	}

	if (enemy)
	{
		FVector velocity{ enemy->GetVelocity() };
		velocity.Z = 0.0f;
		Speed = velocity.Size();
	}
}

void UAlienBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//UE_LOG(LogTemp, Warning, TEXT("speed: %f"), Speed);
	Actor = GetOwningActor();
	if (Actor)
	{
		enemyMonster = Cast<AEnemyMonster>(Actor);

		if (enemyMonster)
		{
			Speed = enemyMonster->vSpeed;
			isAttacking = enemyMonster->vIsAttacking;
			isDead = enemyMonster->vIsDead;
		}
	}
}
