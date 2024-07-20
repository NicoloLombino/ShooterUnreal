// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify1.generated.h"

/**
 * 
 */
UCLASS()
class LEZIONIUDEMY2_API UAnimNotify1 : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify1();

protected:
	virtual void Notify(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation) override;
	
	USoundWave* audio0;
	USoundWave* audio1;
	USoundWave* audio2;

	TArray<USoundWave*> audios;
		
	UPROPERTY(EditAnywhere)
		bool getSurface;

	UPROPERTY(EditAnywhere)
		int audioNum = -1;

	// per istanziare il suono nella posizione del player
	void PlayAudio(const UObject* object, FVector location, int ref);

	int GetPMaterial(USkeletalMeshComponent* meshComp);


};
