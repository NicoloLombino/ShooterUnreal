// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "EnemyMonster.generated.h"

UCLASS()
class LEZIONIUDEMY2_API AEnemyMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void DetectSphereOverlap(
		UPrimitiveComponent* overlappedComponent,
		AActor* otherActor,
		UPrimitiveComponent* otherComp,
		int32 otherBodyIndex,
		bool bFromSweep,
		const FHitResult& sweepResult );


	UFUNCTION(BlueprintCallable)
		void SetStunned(bool stunned);

	// AI

	UPROPERTY(EditAnywhere, Category = "Behaviour Tree", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* behaviourTree;

	// punto 1 pattuglia (patrol point)
	UPROPERTY(EditAnywhere, Category = "Behaviour Tree", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
		FVector patrolPoint;
	// punto 2 pattuglia (patrol point)
	UPROPERTY(EditAnywhere, Category = "Behaviour Tree", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
		FVector patrolPoint2;

	class AEnemyController* enemyController;

	// Overlap Trigger
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	class USphereComponent* detectSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* hitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* ScreamMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* Attack1Montage;

	FTimerHandle hitReactTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		float hitReactTimerMin = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		float hitReactTimerMax = 3.0f;

	// stun system
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		bool isStunned = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		float stunChance = 0.5f;

	// attack system
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		bool isInAttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
		class USphereComponent* combatRangeSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		AActor* Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		float RagePercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		bool IsInRage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		bool HasScream;


protected:

	// HUD - Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<class UUserWidget> theWidgetLife;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		UUserWidget* widgetLife;

	// >TIMERS
	FTimerHandle gunTimer;

	UFUNCTION()
		void FAttackTimer();

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
		float maxHealth = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float currentHealth = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
		float EnemySpeed = 100.0f;

	void ShowHealthBar_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
		void HideHealthBar();

	FTimerHandle healthBarTimer;
	float healthBarDisplayTime = 4.0f;
	bool ishealthVisible = false;

public:

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float damage = 20.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	USoundBase* ScreamSound;


	void SetHealth(float healthValue);

	UFUNCTION(BlueprintNativeEvent)
		void ShowHealthBar();

	// values
	float vSpeed = 0.0f;
	bool vIsDead = false;
	bool vIsAttacking = false;

	float GetHealth() { return currentHealth; }

	FORCEINLINE UBehaviorTree* GetBehaviorTree() const
	{
		return behaviourTree;
	}

	void PlayHitMontage(FName section, float playRate = 1.0f);

	bool canHitReact = true;
	void ResetHitReactTimer();

	UFUNCTION()
	void CombatRangeOverlap(
		UPrimitiveComponent* overlappedComponent,
		AActor* otherActor,
		UPrimitiveComponent* otherComp,
		int32 otherBodyIndex,
		bool bFromSweep,
		const FHitResult& sweepResult
	);

	UFUNCTION()
	void CombatRangeEndOverlap(
		UPrimitiveComponent* overlappedComponent,
		AActor* otherActor,
		UPrimitiveComponent* otherComp,
		int32 otherBodyIndex
	);

	UFUNCTION(BlueprintCallable)
	void AttackCollisionHit(AActor* otherActor, float InDamage);

	UFUNCTION(BlueprintCallable)
		void Scream();

	UFUNCTION(BlueprintCallable)
		void EnableMovement();

	UFUNCTION(BlueprintCallable)
		void DisableMovement();

	UFUNCTION(BlueprintCallable)
		void SetTargetForBehavior(AActor* TargetToGive);
};
