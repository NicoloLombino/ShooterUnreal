// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "PlayerBase.generated.h"

UCLASS()
class LEZIONIUDEMY2_API APlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	float capsuleRadius;
	float capsuleHalfHeigh;

	float jumpVelocity;
	float jumpControlValue;

	float myDeltaTime;  // recuperare il valore del delta time per riutilizzarlo

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	bool isGunVisible;
	bool isCrouching;

	bool isRunning;

	bool isMachineGun;

	float damageFromVomit = 5.0f;

protected:
	// accedere al braccio della telecamera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		USpringArmComponent* armComp;

	// cose su UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	/* 
	*  BlueprintReadWrite = visibile e modificabile da blueprint
	*  BlueprintReadOnly = solo visibile da blueprint
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		UCameraComponent* camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
		float mouseSpeedX;									
															
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
		float mouseSpeedY;

	// weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<class AGun> weapon0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class AGun* gun;

	// HUD -Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<class UUserWidget> theWidgetTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		UUserWidget* currentWidgetTarget;

	void ShowTarget();

	// >TIMERS
	FTimerHandle gunTimer;
	FTimerHandle VomitDamageTimer;

	UFUNCTION()
		void FGunTimer();

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float maxHealth = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float currentHealth = 100.0f;


public:
	
	// axis
	void FMoveToX(float value);
	void FMoveToY(float value);

	void FRotate(float value);
	void FLookUpDown(float value);
	void FZoom(float value);

	//actions
	void FToRun();
	void FNotToRun();
	void FToJump();
	void FNotToJump();

	void FToCrouch();
	void FNotToCrouch();

	void FToGun();
	void FToShoot();
	void FNotToShoot();
	void FToMachineGun();

	void TakeDamadeFromVomit();

	float vSpeed;
	float vCrouch;
	bool vIsInAir;
	bool isTargetVisible;

	bool vIsDead;
	bool allowToShoot;
	bool shootToTarget;

	// regular movement speed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (allowPrivateAccess = "true"))
	float baseMovementSpeed;

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float damage = 20.0f;

	UFUNCTION(BlueprintCallable)
	void SetHealth(float healthValue);

	UFUNCTION(BlueprintCallable)
	void HandleEnterOnVomitArea(float InDamageFromVomit, float InVomitRateTime);
	UFUNCTION(BlueprintCallable)
	void HandleExitFromVomitArea();
	UFUNCTION(BlueprintCallable)
	void SetTargetWidgetVisibility();

};
