// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PlayerBase.h"
#include "Components/ArrowComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class LEZIONIUDEMY2_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Shoot(AActor* actor, bool toTarget, float InDamage);


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gun")
		USkeletalMeshComponent* gunComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gun")
		UArrowComponent* arrow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
		UParticleSystemComponent* spawnEffectComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
		USoundWave* audio;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Particle")
	UParticleSystem* spawnEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Particle")
		UParticleSystem* impactBlood;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Particle")
		UParticleSystem* impactBullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Particle")
	UMaterialInterface* bulletDecal;

	void PlayAudio(const UObject* object, FVector location);

	void DrawBulletImpact();
	FHitResult hitResult;

public:

	AActor* PlayerRef;

};
