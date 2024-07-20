// Fill out your copyright notice in the Description page of Project Settings.



#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "EnemyMonster.h"


// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//----------------------MESH-------------------
	//gunComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	//RootComponent = gunComp;
	//ConstructorHelpers::FObjectFinder<USkeletalMesh> gunMeshObj(TEXT("Skeleton'/Game/CoseMie/Prefabs/beretta_m9_fbx/beretta_m9/beretta_m9_Skeleton.beretta_m9_Skeleton'"));
	//if (gunMeshObj.Object)
	//{
	//	gunComp->SetSkeletalMesh(gunMeshObj.Object);
	//}

	//----------------------ARROW-------------------(per vedere dove spara)
	arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow")); 
	arrow->SetupAttachment(gunComp, FName("shot"));

	//----------------------PARTICLES-------------------
	/*
	spawnEffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Spawn Particles"));
	spawnEffectComp->bAutoActivate = false;
	spawnEffectComp->SetupAttachment(gunComp, FName("shot"));
	*/
	/*
	ConstructorHelpers::FObjectFinder<UParticleSystem> spawnEffectObj(TEXT("C:/PROGETTI UNREAL 5/AlienGameUnreal/Content/ContenutiIniziali/Particles/P_Explosion.uasset"));
	if (spawnEffectObj.Object)
	{
		spawnEffect = spawnEffectObj.Object;
	}
	spawnEffectComp->SetTemplate(spawnEffect);
	*/
	//per gli altri particellari
	/*
	ConstructorHelpers::FObjectFinder<UParticleSystem> impactBloodObj(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Sparks.P_Sparks'"));
	if (impactBloodObj.Object)
	{
		impactBlood = impactBloodObj.Object;
	}
	*/
	/*
	ConstructorHelpers::FObjectFinder<UParticleSystem> impactBulletObj(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Smoke.P_Smoke'"));
	if (impactBulletObj.Object)
	{
		impactBullet = impactBulletObj.Object;
	}
	*/
	// per il materiale ----> decal
	/*
	ConstructorHelpers::FObjectFinder<UMaterialInterface> bulletDecalObj(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Smoke.P_Smoke'"));
	if (bulletDecalObj.Succeeded()) // un altro modo di scriverlo
	{
		bulletDecal = bulletDecalObj.Object;
	}
	*/


	//-----------------AUDIO-------------------
	/*
	ConstructorHelpers::FObjectFinder<USoundWave>audio0Obj(TEXT("SoundWave'/Game/CoseMie/Sounds/GunShot.GunShot'"));
	if (audio0Obj.Object)
	{
		audio = audio0Obj.Object;
	}
	*/
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::Shoot(AActor* actor, bool toTarget, float InDamage)
{
	FString actorClassName = actor->GetClass()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("ClassName = %s"), *actorClassName);

	if(actorClassName == "BP_PlayerBaseDoozy_C")
	{
		APlayerBase* playerBase = Cast<APlayerBase>(actor);
		if (toTarget && !playerBase->isTargetVisible)
		{
			return; 
		}
	}

	// ACCESSO ALLA FRECCIA

	// direzione della freccia
	FVector arrowLocation = arrow->GetComponentLocation();

	// rotazione della freccia
	FRotator arrowRotation = arrow->GetComponentRotation();

	// Scale della freccia
	FVector arrowScale = FVector(0.9f);

	// PARTICLES
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), spawnEffect, arrowLocation, arrowRotation, arrowScale);

	// SUONO DELLO SPARO
	PlayAudio(arrow, arrowLocation);

	// DIREZIONE DEL TIRO
	FVector start;
	FVector end;


	// il raggio di trasmissione va da start ad end
	// dobbiamo recuperare le informazioni "HIT" in una struttura
	// questa struttura � FHitResult

	FCollisionQueryParams infoProperties;

	// ignorare se il raggio colpisce la propria classe (istanza dell'arma)
	infoProperties.AddIgnoredActor(this);

	// ignorare se il raggio colpisce l'attore che spara
	infoProperties.AddIgnoredActor(actor);

	if (!toTarget)
	{
		// sparare rispetto alla posizione
		start = arrowLocation;

		// direzione (angolo)
		FVector direction = arrowRotation.Vector();

		// poligono di tiro
		end = start + direction * 3000.0f;
	}

	else
	{
		// sparare al bersaglio al centro dello schermo
		FVector2D viewPortSize;

		if (GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->GetViewportSize(viewPortSize);
		}

		FVector2D targetLocation = FVector2D(viewPortSize.X / 2, viewPortSize.Y / 2);

		FVector targetWorldPosition;
		FVector targetWorldDirection;


		bool screenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::
			GetPlayerController(this, 0), targetLocation, targetWorldPosition, targetWorldDirection);

		if (screenToWorld)
		{
			start = targetWorldPosition;
			end = targetWorldPosition + targetWorldDirection * 5000.0f;
		}

		// sparo -> visualizzare una linea rossa che mostra l'andamento dello sparo
		// diametro: 1.0f
		// durata del display: 0.5f secondi
		// #include "Engine/Public/DrawDebugHelpers.h"

		DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, 2.0f, 0, 0.1f);

		bool targetArchived = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, infoProperties);
		if (targetArchived)
		{
			UE_LOG(LogTemp, Warning, TEXT("ClassName = %s"), *hitResult.GetComponent()->GetName());
			// quale attore ha raggiunto il tiro?
			AActor* actor2 = hitResult.GetActor();
			//if (actor2 != nullptr)
			if (actor2)
			{
				FString className = actor2->GetClass()->GetName();
				UE_LOG(LogTemp, Warning, TEXT("ClassName = %s"), *className);

				bool isPlayer = actor2->ActorHasTag("Player");
				bool isEnemy = actor2->ActorHasTag("Enemy");

				if (isPlayer)
				{
					// visualizzare particella sangue (impactBlood)
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactBlood, hitResult.Location, hitResult.ImpactNormal.Rotation());

					// decrease the health of enemy
					APlayerBase* enemy = Cast<APlayerBase>(actor2);

					enemy->SetHealth(enemy->damage);
				}
				else if (isEnemy)
				{
					// visualizzare particella sangue (impactBlood)
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactBlood, hitResult.Location, hitResult.ImpactNormal.Rotation());
					// decrease the health of enemy
					AEnemyMonster* enemy = Cast<AEnemyMonster>(actor2);

					if (PlayerRef)
					{
						if(!enemy->Target)
						{
							enemy->SetTargetForBehavior(PlayerRef);
						}
					}

					// show the health bar of enemy
					if (enemy->GetHealth() > 0)
					{
						enemy->ShowHealthBar();
					}

					if (!enemy->vIsDead)
					{
						enemy->ShowHealthBar();
						
						const float rndStun = FMath::FRandRange(0, 1);
						if (rndStun <= enemy->stunChance)
						{
							//stun the enemy
							/*enemy->isStunned = true;*/
							enemy->SetStunned(true);
							enemy->PlayHitMontage("Hit");
						}
					}
					// enemy's death
					enemy->SetHealth(InDamage);
				}
				else
				{
					// show particles: impact (impactBullet)
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactBullet, hitResult.Location, hitResult.ImpactNormal.Rotation(), true);

					// decal
					DrawBulletImpact();

					// give a physical impulse at impact
					// remember to put the physical simulation on true

					FString s = actor2->GetClass()->GetName();
					// 
					if (s == "StaticMeshActor")
					{
						UMeshComponent* comp = Cast<UStaticMeshComponent>(hitResult.GetActor()->GetRootComponent());

						// position of impulse x impact force
						if (comp)
						{
							comp->AddImpulse(hitResult.Location * 100.0f);
						}
					}
				}

			}
		}
	}



}

void AGun::PlayAudio(const UObject* object, FVector location)
{
	if (audio != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(object, audio, location);
	}
}

void AGun::DrawBulletImpact()
{
	ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(hitResult.Location, hitResult.ImpactNormal.Rotation());

	if (decal)
	{
		// give the material to decal
		decal->SetDecalMaterial(bulletDecal);

		// set the visibility of decal
		// lower value = more visibility from long distance
		// if value == 0 -> we can see from all distance
		decal->GetDecal()->FadeScreenSize = 0.0f;

		// decal duration
		// 0 = infinity time
		decal->SetLifeSpan(0.0f);

		FVector diameter = FVector(5.0f);
		// FVector diameter = FVector(FMath::RandRange (10.0f, 50.0f); random size
		decal->GetDecal()->DecalSize = diameter;

		// rotate after the spawn to had the correct design
		decal->SetActorRotation(hitResult.ImpactNormal.Rotation());

	}
}

