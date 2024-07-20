 // Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "Gun.h"

 // Sets default values
APlayerBase::APlayerBase()
{
	// variabili
	capsuleRadius = 34.0f;
	capsuleHalfHeigh = 88.0f;
	jumpVelocity = 600.0f;
	jumpControlValue = 0.2f;
	mouseSpeedX = 10.0f;
	mouseSpeedY = 10.0f;
	vCrouch = 100.0f;
	vSpeed = 0.0f;
	isGunVisible = false;
	isCrouching = false;
	vIsInAir = false;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//-----------------CAPSULE COMPONENTS-------------------
	// radius
	GetCapsuleComponent()->SetCapsuleRadius(capsuleRadius);

	// half height
	GetCapsuleComponent()->SetCapsuleHalfHeight(capsuleHalfHeigh);

	//-----------------CHARACTER MOVEMENT-------------------
	// jump
	GetCharacterMovement() -> JumpZVelocity = jumpVelocity;

	// controllo del player durante il salto
	GetCharacterMovement()->AirControl = jumpControlValue;

	// il personaggio si gira in direzione dell'input
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// controller
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	/*GetCharacterMovement()->MaxWalkSpeed = 300.0f;*/ // velore di dafault originale 600.0f

	//-----------------SKELETAL MESH-------------------------
	// SkeletalMesh'/Game/CoseMie/Actors/Doozy/Ch19_nonPBR_1_.Ch19_nonPBR_1_'

	// recuperare la reference dello skeletal mesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj(TEXT("/Game/CoseMie/Actors/Doozy/Ch19_nonPBR_1_.Ch19_nonPBR_1_"));
	if (MeshObj.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshObj.Object);

		// location dello skeletal mesh component
		// abbassare il personaggio per farlo corrispondere all'altezza della capsula
		GetMesh()->SetRelativeLocation(FVector(0, 0, -85.0f));

		// rotazione dello skeletal mesh del personaggio
		// per posizionare lo skeletal mesh nell direzione della freccia blu
		// si usano:  PITCH - YAW - ROLL = Y, Z, X (e non X, Y, Z)
		GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

		// modificare lo scale del personaggio
		// attualmente non mi serve perchè va già bene di default (ha valori casuali)
		//GetMesh()->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));

		// collision (i do it by details)
		//GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	}

	//-----------------HUD - Widget-------------------------
	ConstructorHelpers::FClassFinder<UUserWidget>theWidgetObj(TEXT("/Game/CoseMie/UI/Blueprint/BP_TargetWidget"));
	if (theWidgetObj.Class) theWidgetTarget = theWidgetObj.Class;


	//-----------------CAMERA COMPONENT-------------------------
	armComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm Component"));

	// attaccare armComp al root component
	armComp->SetupAttachment(RootComponent);

	// giriamo il braccio con il controller
	armComp->bUsePawnControlRotation = true;

	// decentrare il personaggio
	armComp->SocketOffset = FVector(0, 50, 50);

	// per evitare che la camera trovi le collisioni (di default a true e io lo voglio così)
	//armComp->bDoCollisionTest = false;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(armComp);

	//-----------------SET THIS AS PLAYER0-------------------------
	// player0 = player principale su unreal
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//-------------------------- ANIMATIONS ---------------------------------------
	// Animation Mode            (si possono cambiare -> (F12 su EAnimationMode))
	// reference BP per animzione ---->    AnimBlueprint'/Game/CoseMie/Actors/Doozy/BP_DoozyAnimInstance.BP_DoozyAnimInstance'

	// per mettere le reference ai componenti necessari
	/*
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint); // già di default
	ConstructorHelpers::FObjectFinder<UBlueprint>BP(TEXT("AnimBlueprint'/Game/CoseMie/Actors/Doozy/BP_DoozyAnimInstance.BP_DoozyAnimInstance'"));
	GetMesh()->AnimClass = BP.Object->GeneratedClass;

	//-------------------------- WEAPONS ---------------------------------------
	// usando il blueprint
	ConstructorHelpers::FObjectFinder<UBlueprint>BP_Weapon0Obj(TEXT("Blueprint'/Game/CoseMie/Blueprint/BP_Gun.BP_Gun'"));   
	//ConstructorHelpers::FObjectFinder<UClass>weapon0Obj(TEXT(""));   // usando la classe
	if (BP_Weapon0Obj.Object)
	{
		weapon0 = BP_Weapon0Obj.Object->GeneratedClass;
	}
	*/
}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
	//-------------------------- ARMI ---------------------------------------
	// rotation, location, scale  (sono quelli sotto in ordine)
	/*
	FTransform weaponTransform = FTransform(FRotator(0,0,0), FVector(0,0,0), FVector(1,1,1));
	FActorSpawnParameters weaponParams;
	weaponParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// My HUD = SpawnActor<AHUD>(NewHUDClass ,this, instigator);  // spawn di un actor
	gun = GetWorld()->SpawnActor<AGun>(weapon0, FTransform(weaponTransform), weaponParams);
	gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Gun"));
	gun->SetActorHiddenInGame(true);
	gun->PlayerRef = this;
	*/

	//-------------------------- HUD - Widget ---------------------------------------
	/*
	if (theWidgetTarget)
	{
		currentWidgetTarget = CreateWidget<UUserWidget>(GetWorld(), theWidgetTarget);
		if (currentWidgetTarget)
		{
			// aggiungere il widget al viewport
			currentWidgetTarget->AddToViewport();
			currentWidgetTarget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	*/
}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	myDeltaTime = DeltaTime;

	// control for jump
	vIsInAir = GetCharacterMovement()->IsFalling();

	// recuperare la velocità di spostamento del personaggio
	FVector V = GetVelocity();
	vSpeed = V.Size();

	ShowTarget();
}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//-----------------AXIS-------------------------

	PlayerInputComponent->BindAxis("MoveToX", this, &APlayerBase::FMoveToX);
	PlayerInputComponent->BindAxis("MoveToY", this, &APlayerBase::FMoveToY);
	PlayerInputComponent->BindAxis("Rotate", this, &APlayerBase::FRotate);
	PlayerInputComponent->BindAxis("LookUpDown", this, &APlayerBase::FLookUpDown);
	PlayerInputComponent->BindAxis("Zoom", this, &APlayerBase::FZoom);

	//-----------------ACTION-------------------------

	PlayerInputComponent->BindAction("ToRun", IE_Pressed, this, &APlayerBase::FToRun);
	PlayerInputComponent->BindAction("ToRun", IE_Released, this, &APlayerBase::FNotToRun);

	PlayerInputComponent->BindAction("ToJump", IE_Pressed, this, &APlayerBase::FToJump);
	PlayerInputComponent->BindAction("ToJump", IE_Released, this, &APlayerBase::FNotToJump);

	PlayerInputComponent->BindAction("ToCrouch", IE_Pressed, this, &APlayerBase::FToCrouch);
	PlayerInputComponent->BindAction("ToCrouch", IE_Released, this, &APlayerBase::FNotToCrouch);

	// toGun
	PlayerInputComponent->BindAction("ToGun", IE_Pressed, this, &APlayerBase::FToGun);

	// toShoot
	PlayerInputComponent->BindAction("ToShoot", IE_Pressed, this, &APlayerBase::FToShoot);
	PlayerInputComponent->BindAction("ToShoot", IE_Released, this, &APlayerBase::FNotToShoot);
	PlayerInputComponent->BindAction("ToMachineGun", IE_Pressed, this, &APlayerBase::FToMachineGun);
}

void APlayerBase::ShowTarget()
{
	if (isTargetVisible)
	{
		if (!currentWidgetTarget->IsVisible() && !isRunning && !isCrouching && !vIsInAir)
		{
			currentWidgetTarget->SetVisibility(ESlateVisibility::Visible);
			UE_LOG(LogTemp, Warning, TEXT("VISIBLE"));


			// lunghezza del braccio
			armComp->TargetArmLength = 60.f;

			bUseControllerRotationPitch = true;
			bUseControllerRotationYaw = true;
		}

		else if (isRunning || isCrouching || vIsInAir)
		{
			UE_LOG(LogTemp, Warning, TEXT("NOOOOOO"));
			currentWidgetTarget->SetVisibility(ESlateVisibility::Hidden);
			bUseControllerRotationPitch = false;
			bUseControllerRotationYaw = false;
			FRotator r = GetCapsuleComponent()->GetRelativeRotation();
			float yaw = r.Yaw;
			GetCapsuleComponent()->SetRelativeRotation(FRotator(0, yaw, 0));
		}
	}
	else if(currentWidgetTarget->IsVisible())
	{
		currentWidgetTarget->SetVisibility(ESlateVisibility::Hidden);
		bUseControllerRotationPitch = false;
		bUseControllerRotationYaw = false;
		FRotator r = GetCapsuleComponent()->GetRelativeRotation();
		float yaw = r.Yaw;
		GetCapsuleComponent()->SetRelativeRotation(FRotator(0, yaw, 0));
	}
}

void APlayerBase::FGunTimer()
{
	if (isGunVisible && isMachineGun && allowToShoot)
	{
		gun->Shoot(this, shootToTarget, damage);
	}
}

void APlayerBase::SetHealth(float healthValue)
{
	if (currentHealth <= 0.0f) return;

	currentHealth -= healthValue;
	if (currentHealth <= 0.0f)
	{
		// player death
		vIsDead = true;
		allowToShoot = false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Health: %f"), currentHealth);
	}
}

void APlayerBase::FMoveToX(float value)
{
	if (Controller && value != 0.0f)
	{
		FRotator rotation = Controller->GetControlRotation();
		FRotator yawRotation(0.0f, rotation.Yaw, 0.0f);
		FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(direction, value);
	}
}

void APlayerBase::FMoveToY(float value)
{
	if (Controller && value != 0.0f)
	{
		FRotator rotation = Controller->GetControlRotation();
		FRotator yawRotation(0.0f, rotation.Yaw, 0.0f);
		FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(direction, value);
	}
}

void APlayerBase::FToRun()
{
	isRunning = true;
	allowToShoot = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	UE_LOG(LogTemp, Warning, TEXT("RUN"));
}

void APlayerBase::FNotToRun()
{
	isRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	UE_LOG(LogTemp, Warning, TEXT("WALK"));
}

void APlayerBase::FRotate(float value)
{
	AddControllerYawInput(value * mouseSpeedX * myDeltaTime);
}

void APlayerBase::FLookUpDown(float value)
{
	AddControllerPitchInput(value * -mouseSpeedY * myDeltaTime);  // con il - ho messo la camera "normale"
}

void APlayerBase::FZoom(float value)
{
	if (currentWidgetTarget->IsVisible())
	{
		return;
	}

	// lunghezza del braccio
	float f = armComp->TargetArmLength - (value * 40.0f);
	if (f < 130) f = 130.0f;
	if (f > 2000) f = 2000.0f;
	armComp->TargetArmLength = f;
}

void APlayerBase::FToJump()
{
	// le funzioni Jump() e StopJumping() esistono di default su unreal
	Jump();
}
void APlayerBase::FNotToJump()
{
	//StopJumping();
	// per ora non serve perchè verrà gestito con un'animazione
}

void APlayerBase::FToCrouch()
{
	// le funzioni Crouch() e UnCrouch() esistono di default su unreal
	//Crouch();

	isCrouching = true;
	vCrouch = 200.0f;
}
void APlayerBase::FNotToCrouch()
{
	//UnCrouch();
	// per ora non serve perchè verrà gestito con un'animazione
	
	isCrouching = false;
	if (isGunVisible) vCrouch = 0.0f;
	else vCrouch = 100.0f;
}

void APlayerBase::FToGun()
{
	isGunVisible = !isGunVisible;

	if (isGunVisible)
	{
		if (isCrouching) vCrouch = 200.0f;
		else vCrouch = 0.0f;

		gun->SetActorHiddenInGame(false);
	}
	else
	{
		vCrouch = 100.0f;
		if (isCrouching) vCrouch = 200.0f;
		else vCrouch = 100.0f;
		gun->SetActorHiddenInGame(true);
	}
	isTargetVisible = isGunVisible;
}

void APlayerBase::FToShoot()
{
	if (!isGunVisible)
	{
		return;
	}

	if(vIsDead || vCrouch || vIsInAir || isRunning) return;
	
	shootToTarget = true;
	allowToShoot = true;
	/*gun->Shoot(this, shootToTarget);*/
	UE_LOG(LogTemp, Warning, TEXT("SHOOTING"));
	if (!isMachineGun && allowToShoot)
	{
		gun->Shoot(this, shootToTarget, damage);
	}
	else if (allowToShoot)
	{
		GetWorldTimerManager().SetTimer(gunTimer, this, &APlayerBase::FGunTimer, 0.1f, true);
	}
}

void APlayerBase::FNotToShoot()
{
	if (isMachineGun)
	{
		GetWorldTimerManager().ClearTimer(gunTimer);
	}
}

void APlayerBase::FToMachineGun()
{
	isMachineGun = !isMachineGun;
}

void APlayerBase::HandleEnterOnVomitArea(float InDamageFromVomit, float InVomitRateTime)
{
	damageFromVomit = InDamageFromVomit;
	GetWorldTimerManager().SetTimer(VomitDamageTimer, this, &APlayerBase::TakeDamadeFromVomit, InVomitRateTime, true);
}

void APlayerBase::HandleExitFromVomitArea()
{
	GetWorldTimerManager().ClearTimer(VomitDamageTimer);
}

 void APlayerBase::SetTargetWidgetVisibility()
 {
	if(isTargetVisible)
	{
		currentWidgetTarget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		currentWidgetTarget->SetVisibility(ESlateVisibility::Hidden);
	}
 }

 void APlayerBase::TakeDamadeFromVomit()
{
	SetHealth(damageFromVomit);
}

