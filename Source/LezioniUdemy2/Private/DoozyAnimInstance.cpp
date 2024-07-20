// Fill out your copyright notice in the Description page of Project Settings.


#include "DoozyAnimInstance.h"

void UDoozyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	Actor = GetOwningActor();
	if (Actor)
	{
		playerBase = Cast<APlayerBase>(Actor);

		if (playerBase)
		{
			Speed = playerBase->vSpeed;
			Crouch = playerBase->vCrouch;
			isInAir = playerBase->vIsInAir;
			isDead = playerBase->vIsDead;
		}
	}
}