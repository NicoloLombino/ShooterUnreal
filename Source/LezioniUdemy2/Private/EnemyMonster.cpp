// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMonster.h"
#include "EnemyController.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"  // per vedere wordPatrolPoint come debug
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "PlayerBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyMonster::AEnemyMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//-------------------------- COMBAT ---------------------------------------
	detectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectSphere"));
	detectSphere->SetupAttachment(GetRootComponent());

	combatRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatRangeSphere"));
	combatRangeSphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AEnemyMonster::BeginPlay()
{
	Super::BeginPlay();
	
	//-------------------------- HUD - Widget ---------------------------------------
	if (theWidgetLife)
	{
		widgetLife = CreateWidget<UUserWidget>(GetWorld(), theWidgetLife);
		if (widgetLife)
		{
			// aggiungere il widget al viewport
			widgetLife->AddToViewport();
			widgetLife->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	//-------------------------- AI ---------------------------------------
	enemyController = Cast<AEnemyController>(GetController());

	currentHealth = maxHealth;

	FVector worldPatrolPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), patrolPoint);
	DrawDebugSphere(GetWorld(), worldPatrolPoint, 25.0f, 12, FColor::Green, true);

	FVector worldPatrolPoint2 = UKismetMathLibrary::TransformLocation(GetActorTransform(), patrolPoint2);
	DrawDebugSphere(GetWorld(), worldPatrolPoint2, 25.0f, 12, FColor::Green, true);

	// the vector became the value
	if (enemyController)
	{
		enemyController->GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolPoint"), worldPatrolPoint);
		enemyController->GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolPoint2"), worldPatrolPoint2);
		enemyController->RunBehaviorTree(behaviourTree);
	}

	//-------------------------- COMBAT ---------------------------------------
	detectSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMonster::DetectSphereOverlap);

	combatRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMonster::CombatRangeOverlap);
	combatRangeSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyMonster::CombatRangeEndOverlap);

	//-------------OTHERS-----------------------
	EnemySpeed = GetCharacterMovement()->GetMaxSpeed();
}

// Called every frame
void AEnemyMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyMonster::DetectSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor == nullptr) return;

	auto character = Cast<APlayerBase>(otherActor);
	if (character)
	{
		if (!HasScream && !vIsDead)
		{
			UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
			if (animInstance)
			{
				FVector EnemyLocation = GetActorLocation();
				FVector TargetLocation = character->GetActorLocation();
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(EnemyLocation, TargetLocation);
				SetActorRotation(LookAtRotation);
				GetCharacterMovement()->DisableMovement();
				if(ScreamMontage && currentHealth >= maxHealth)
				{
					animInstance->Montage_Play(ScreamMontage, 1);
					if(ScreamSound)
					{
						UGameplayStatics::PlaySound2D(this, ScreamSound);
					}
				}
				else
				{
					EnableMovement();
				}
				SetTargetForBehavior(character);

			}
		}
	}
}

void AEnemyMonster::SetStunned(bool stunned)
{
	isStunned = stunned;

	if (enemyController)
	{
		enemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("Stunned"), stunned);
	}
}

void AEnemyMonster::FAttackTimer()
{
}


void AEnemyMonster::ShowHealthBar_Implementation()
{
	GetWorldTimerManager().ClearTimer(healthBarTimer);
	GetWorldTimerManager().SetTimer(healthBarTimer, this, &AEnemyMonster::HideHealthBar, healthBarDisplayTime);
}


void AEnemyMonster::PlayHitMontage(FName section, float playRate)
{
	if(!hitMontage) return;
	
	if (canHitReact)
	{
		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (animInstance)
		{
			animInstance->Montage_Play(hitMontage, playRate);
			animInstance->Montage_JumpToSection(section, hitMontage);
		}
	}
	canHitReact = false;
	const float hitReactTime{ FMath::FRandRange(hitReactTimerMin, hitReactTimerMax) };
	GetWorldTimerManager().SetTimer(hitReactTimer, this, &AEnemyMonster::ResetHitReactTimer, hitReactTime);
}

void AEnemyMonster::ResetHitReactTimer()
{
	canHitReact = true;
}

void AEnemyMonster::CombatRangeOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor == nullptr) return;

	APlayerBase* playerBase = Cast<APlayerBase>(otherActor);
	if (playerBase)
	{
		isInAttackRange = true;
		if (enemyController)
		{
			enemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsInAttackRange"), true);
		}
	}
}

void AEnemyMonster::CombatRangeEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (otherActor == nullptr) return;

	APlayerBase* playerBase = Cast<APlayerBase>(otherActor);
	if (playerBase)
	{
		isInAttackRange = false;
		if (enemyController)
		{
			enemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsInAttackRange"), false);
		}
	}
}

void AEnemyMonster::AttackCollisionHit(AActor* otherActor,  float InDamage)
{
	if (otherActor == nullptr) return;

	APlayerBase* playerBase = Cast<APlayerBase>(otherActor);
	if (playerBase)
	{
		playerBase->SetHealth(InDamage);
	}
}

void AEnemyMonster::Scream()
{
	HasScream = true;
	if (enemyController)
	{
		enemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("HasScream"), true);
	}
}

void AEnemyMonster::EnableMovement()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AEnemyMonster::DisableMovement()
{
	GetCharacterMovement()->DisableMovement();
}

void AEnemyMonster::SetTargetForBehavior(AActor* TargetToGive)
{
	enemyController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), TargetToGive);
	Target = TargetToGive;

}

void AEnemyMonster::SetHealth(float healthValue)
{
	if (currentHealth <= 0) return;

	currentHealth -= healthValue;
	if (currentHealth <= 0)
	{
		vIsDead = true;
		enemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"), true);
	}
	else
	{
		if (FMath::FRandRange(0, 1) < RagePercentage && !IsInRage)
		{
			IsInRage = true;
			GetCharacterMovement()->MaxWalkSpeed += 300.f;
			EnemySpeed = GetCharacterMovement()->MaxWalkSpeed;
			damage += 10;
			UE_LOG(LogTemp, Warning, TEXT("RAGE"));
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Enemy Current Health: %f"), currentHealth);
}


