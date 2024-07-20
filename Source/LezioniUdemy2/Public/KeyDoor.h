// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSequenceActor.h"
#include "LevelSequence/Public/LevelSequence.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "KeyDoor.generated.h"


UENUM(BlueprintType)
enum class EKeyType : uint8  // enum di int
{
	KT_Jump UMETA(DisplayName = "Jump Higher"),
	KT_Target UMETA(DisplayName = "Show Target"),
	KT_Door UMETA(DisplayName = "Open Door")
};

UCLASS()
class LEZIONIUDEMY2_API AKeyDoor : public AActor
{
	GENERATED_BODY()
	
public:	

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* primitiveComp, AActor* actor, UPrimitiveComponent* primitiveComp1,
			int i, bool bArgs, const FHitResult& hitResult);
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* primitiveComp, AActor* actor, UPrimitiveComponent* primitiveComp1,
			int i);
	// Sets default values for this actor's properties
	AKeyDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
		UBoxComponent* box;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
		UStaticMeshComponent* keyMeshComp;

	UStaticMesh* key;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
		UParticleSystemComponent* particleComp;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
		UParticleSystem* particles;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
		USoundWave* audio0;
	TArray<USoundWave*> audios;
	void PlayAudio(const UObject* object, FVector location, int ref);


	// per la rotazione della chiave
	UPROPERTY(EditAnywhere, Category = "Movement")
		float pitchValue = 2.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float yawValue = 2.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float rollValue = 2.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
		bool canRotate = false;

	// Sequence animation
	// sequence animation player
	UPROPERTY()
		ULevelSequencePlayer* sequencePlayer;

	// sequence animation Asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animaton")
		ULevelSequence* sequenceAsset;

	// level sequence with right tag
	void PlayLevelSequence(FString tag);

	// tabella sequence actors
	TArray<AActor*> sequenceActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EKeyType keyType;
};
