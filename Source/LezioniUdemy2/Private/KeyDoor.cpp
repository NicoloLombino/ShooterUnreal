// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyDoor.h"
#include "PlayerBase.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AKeyDoor::AKeyDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//-----------------BOX COLLIDER COMPONENTS-------------------

	box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	RootComponent = box;

	//-----------------EVENTS-------------------  // sono i ontriggerEnter
	box->OnComponentBeginOverlap.AddDynamic(this, &AKeyDoor::OnBeginOverlap);
	box->OnComponentEndOverlap.AddDynamic(this, &AKeyDoor::OnEndOverlap);


	//-----------------STATIC MESH COMPONENT   (che conterr� la chiave)-------------------
	keyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key"));
	keyMeshComp->SetupAttachment(box);
	keyMeshComp->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));

	// Static mesh
	//ConstructorHelpers::FObjectFinder<UStaticMesh>keyObj(TEXT("StaticMesh'/Game/CoseMie/Prefabs/Key/Worn_Key.Worn_Key'"));

	//-----------------PARTICLES-------------------
	particleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles FX"));
	particleComp->SetupAttachment(keyMeshComp);
	particleComp->bAutoActivate = false;

	/*
	ConstructorHelpers::FObjectFinder<UParticleSystem>particlesObj(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Steam_Lit.P_Steam_Lit'"));
	if (particlesObj.Object)
	{
		particles = particlesObj.Object;
		particleComp->SetTemplate(particles);
	}
	*/

	//-----------------AUDIO-------------------
	/*
	ConstructorHelpers::FObjectFinder<USoundWave>audio0Obj(TEXT("SoundWave'/Game/CoseMie/Sounds/souno_picking_keys.souno_picking_keys'"));
	if (audio0Obj.Object)
	{
		audio0 = audio0Obj.Object;
		audios = { audio0 };
	}
	*/

	//-----------------TAGS-------------------
	Tags.Add(FName("Key"));

	//-----------------Sequence Player-------------------
	sequencePlayer = CreateDefaultSubobject<ULevelSequencePlayer>(TEXT("Sequence Player"));
	/*
	ConstructorHelpers::FObjectFinder<ULevelSequence>sequenceAssetObj(TEXT("LevelSequence'/Game/CoseMie/Animations/OpenDoor.OpenDoor'"));
	if (sequenceAssetObj.Object)
	{
		sequenceAsset = sequenceAssetObj.Object;
	}
	*/
	//-----------------Sequence Asset-------------------


	// if keyObj exists
	/*
	if (keyObj.Object)
	{
		key = keyObj.Object;
		keyMeshComp->SetStaticMesh(key);
	}
	*/
}

// Called when the game starts or when spawned
void AKeyDoor::BeginPlay()
{
	Super::BeginPlay();
	
	// type of key
	if (keyType == EKeyType::KT_Jump)
	{

	}
}

// Called every frame
void AKeyDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(canRotate)
	{
		FRotator newRotation = FRotator(pitchValue, yawValue, rollValue);
		FQuat  quatRotation = FQuat(newRotation);
		AddActorLocalRotation(quatRotation);
	}

}

void AKeyDoor::PlayAudio(const UObject* object, FVector location, int ref)
{
	/*
	if (ref == 0)
	{
		if (audios[ref] != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(object, audios[ref], location);
		}
	}
	*/
}

void AKeyDoor::PlayLevelSequence(FString tag)
{
	if (UWorld* world = GetWorld())
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelSequenceActor::StaticClass(), sequenceActors);

		for (AActor* seqActor : sequenceActors)  // like a foreach
		{
			ALevelSequenceActor* levSeqActor = Cast<ALevelSequenceActor>(seqActor);
			if (levSeqActor)
			{
				sequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), sequenceAsset,
					FMovieSceneSequencePlaybackSettings(), levSeqActor);

				if (sequencePlayer)
				{
					sequencePlayer->Play();
					break;
				}
			}
		}
	}
}

void AKeyDoor::OnBeginOverlap(UPrimitiveComponent* primitiveComp, AActor* actor,
	UPrimitiveComponent* primitiveComp1, int i, bool bArgs, const FHitResult& hitResult)
{
	 FString s = actor->GetClass()->GetName();
	
	 UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %s"), *s);
	 if (s == "BP_PlayerBaseDoozy_C")
	 {
	 	APlayerBase* playerbase = Cast<APlayerBase>(actor);
	 	if (/*ActorHasTag("Jump")*/ keyType == EKeyType::KT_Jump)
	 	{
	 		playerbase->GetCharacterMovement()->JumpZVelocity = 1000.0f;
	 	}
	 	//-----------------Sequence-------------------
	 	else if (/*ActorHasTag("Door")*/keyType == EKeyType::KT_Door)
	 	{
	 		PlayLevelSequence("Door");
	 	}
	
	 	// particles
	 	FVector location = GetActorLocation();
	 	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particles, location);
	 	PlayAudio(keyMeshComp, location, 0);
	 	// destroy the key
	 	Destroy();
	}
}

void AKeyDoor::OnEndOverlap(UPrimitiveComponent* primitiveComp, AActor* actor,
	UPrimitiveComponent* primitiveComp1, int i)
{
}

