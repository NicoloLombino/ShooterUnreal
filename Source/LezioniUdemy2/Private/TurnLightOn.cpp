// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnLightOn.h"

void ATurnLightOn::Interact()
{
	for (ULightComponent* Light : Lights)
	{
		Light->SetVisibility(true);
	}
	BoxCollision->SetGenerateOverlapEvents(false);
}
