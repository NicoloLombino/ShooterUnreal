// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify1.h"
#include "PlayerBase.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"


UAnimNotify1::UAnimNotify1()
{
	/*
	ConstructorHelpers::FObjectFinder<USoundWave>audio0Obj(TEXT("SoundWave'/Game/CoseMie/Sounds/suono_footstep_normale.suono_footstep_normale'"));
	audio0 = audio0Obj.Object;

	ConstructorHelpers::FObjectFinder<USoundWave>audio1Obj(TEXT("SoundWave'/Game/CoseMie/Sounds/suono_footstep_normale.suono_footstep_normale'"));
	audio1 = audio1Obj.Object;

	ConstructorHelpers::FObjectFinder<USoundWave>audio2Obj(TEXT("SoundWave'/Game/CoseMie/Sounds/suono_footstep_neve.suono_footstep_neve'"));
	audio2 = audio2Obj.Object;


	audios = { audio0,audio1,audio2 };
	*/
}

void UAnimNotify1::Notify(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation)
{
	/*
	Super::Notify(meshComp, animation);

	if (!meshComp) return;

	FString animName = animation->GetName();
	FString className = meshComp->GetOwner()->GetClass()->GetName();

	//UE_LOG(LogTemp, Warning, TEXT("Notify : animState =  %s"), *animName);
	//UE_LOG(LogTemp, Warning, TEXT("Notify : className =  %s"), *className);

	//AActor* actor = meshComp->GetOwner();
	//APlayerBase player = Cast<APlayerBase>(actor);

	FVector location = meshComp->GetOwner()->GetActorLocation();


	if (getSurface)
	{
		int N = GetPMaterial(meshComp);
		PlayAudio(meshComp, location, (N));
	}

	else
	{
		PlayAudio(meshComp, location, audioNum);
	}
	*/
}


/// <summary>
/// permette di instanziare il suono in un luogo preciso
/// </summary>
/// <param name=""></param>
/// <param name="location"></param>
/// <param name="ref"></param>
void UAnimNotify1::PlayAudio(const UObject* object, FVector location, int ref)
{
	/*
	if (ref >= 0 && ref < 3)
	{
		if (audios[ref] != nullptr)  // tanto per fare un controllo se non � nullo
		{
			UGameplayStatics::PlaySoundAtLocation(object, audios[ref], location);
		}
	}
	*/	
}

int UAnimNotify1::GetPMaterial(USkeletalMeshComponent* meshComp)
{
	/*
	// punto di partenza del suono
	FVector start = meshComp->GetComponentLocation();
	FVector forward = meshComp->GetOwner()->GetActorForwardVector();

	start += forward * 50.0f;
	start.Z += 30.0f;

	FVector end = start;
	end.Z -= 1000.0f;

	FHitResult hitResult;
	FCollisionQueryParams infoProperties;

	infoProperties.AddIgnoredActor(meshComp->GetOwner());
	infoProperties.bReturnPhysicalMaterial = true;

	//DrawDebugLine(meshComp->GetOwner()->GetWorld(), start, end, FColor::Blue);
	// definisce un raggio per ottenere il tipo di suolo sotto il player
	meshComp->GetOwner()->GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, infoProperties);

	int nMat = 0; // dare un valore iniziale

	if (hitResult.GetActor())
	{
		UPhysicalMaterial* PM = hitResult.PhysMaterial.Get();

		if (PM) // se il valore esiste
		{
			UE_LOG(LogTemp, Warning, TEXT("trovatoooo"));
			if (PM->SurfaceType == EPhysicalSurface::SurfaceType1) nMat = 0;
			if (PM->SurfaceType == EPhysicalSurface::SurfaceType2) nMat = 1;
			if (PM->SurfaceType == EPhysicalSurface::SurfaceType3) nMat = 2;
		}
	}
	return nMat;
	*/
	return 0;
}
