// Fill out your copyright notice in the Description page of Project Settings.

//my includes
#include "MainCharacter.h"
#include "HealthComponent.h"
#include "GameplayBehaviour/Items/MeleeWeapon.h"
#include "Animation/AnimInstance.h"

//engines include
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/PlayerController.h>
#include <Engine/World.h>
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	///SpringArm
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->TargetArmLength = 600.f;
	SpringArmComp->SetupAttachment(RootComponent);

	///Camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	/* Attach the camera to the end of SprinArmComp and let it adjust
	to match the controller orientation*/
	CameraComp->bUsePawnControlRotation = false;

	/*do not rotate the camera when the controller rotates.
	let that just affect the camera*/
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->SetCapsuleSize(48.f, 105.f);

	/// Character Movement
	BaseTurnRate = 65.0f;
	BaseLookUpRate = 65.0f;

	bMovingForward = false;
	bMovingRight = false;

	bLMBDown = false;
	//bAttacking = false;

	//Configure Character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;//Character moves in the direction of the input
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);//... at this rotation rate 
	GetCharacterMovement()->JumpZVelocity = 550.f;
	//GetCharacterMovement()->AirControl = .2f;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	///movement status
	RunningSpeed = 650.f;
	SprintingSpeed = 950.f;

	bShiftKeyDown = false;
	bPressEDown = false;

	//Enums
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	///Character Stats
	//Stamina
	MaxStamina = 350.f;
	Stamina = 120.f;
	StaminaDrainRate = 25.f;
	MinSprintStamina = 50.f;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void AMainCharacter::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;

	if (MovementStatus == EMovementStatus::EMS_Sprinting) {
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}

//void AMainCharacter::SetEquippedWeapon(AMeleeWeapon * WeaponToSet)
//{
//	if (EquippedWeapon) {
//		EquippedWeapon->Destroy();
//	}
//	EquippedWeapon = WeaponToSet;
//}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComp->OnHealthChanged.AddDynamic(this, &AMainCharacter::OnHealthChanged);

}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementStatus == EMovementStatus::EMS_Dead) return;

	float DeltaStamina = StaminaDrainRate * DeltaTime;

	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:
		if (bShiftKeyDown)
		{
			if (Stamina - DeltaStamina <= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
				Stamina -= DeltaStamina;
			}
			else
			{
				Stamina -= DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Sprinting);
			/*if (bMovingForward || bMovingRight)
			{
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
			else
			{
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}*/
		}
		else // Shift key up
		{
			if (Stamina + DeltaStamina >= MaxStamina)
			{
				Stamina = MaxStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftKeyDown)
		{
			if (Stamina - DeltaStamina <= 0.f)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				Stamina = 0;
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			else
			{
				Stamina -= DeltaStamina;
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
				/*if (bMovingForward || bMovingRight)

					{
					SetMovementStatus(EMovementStatus::EMS_Sprinting);
				}
				else
				{
					SetMovementStatus(EMovementStatus::EMS_Normal);
				}*/
			}
		}
		else // Shift key up
		{
			if (Stamina + DeltaStamina >= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
				Stamina += DeltaStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_Exhausted:
		if (bShiftKeyDown)
		{
			Stamina = 0.f;
		}
		else // Shift key up
		{
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	case EStaminaStatus::ESS_ExhaustedRecovering:
		if (Stamina + DeltaStamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
			Stamina += DeltaStamina;
		}
		else
		{
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	default:
		;
	}

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	///Bind Action
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::ShiftKeyUp);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMainCharacter::EndCrouch);
	PlayerInputComponent->BindAction("EToEquip", IE_Pressed, this, &AMainCharacter::PressEDown);
	PlayerInputComponent->BindAction("EToEquip", IE_Released, this, &AMainCharacter::PressEUp);
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMainCharacter::LMB);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMainCharacter::LMBup);


	///Bind Axis Type
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainCharacter::LookUpAtRate);
}

void AMainCharacter::MoveForward(float Value)
{
	bMovingForward = false;
	if ((Controller != nullptr) && (Value != 0.0f) && (!bAttacking)) {
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		bMovingForward = true;
	}
}

void AMainCharacter::MoveRight(float Value)
{
	bMovingRight = false;

	if ((Controller != nullptr) && (Value != 0.f) && (!bAttacking)) {
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);

		bMovingRight = true;
	}
	
}

void AMainCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AMainCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AMainCharacter::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

void AMainCharacter::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

void AMainCharacter::PressEDown()
{
	bPressEDown = true;

	if (ActiveOverlappingItem) {
		AMeleeWeapon* Weapon = Cast<AMeleeWeapon>(ActiveOverlappingItem);
		if (Weapon) {
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
}

void AMainCharacter::PressEUp()
{
	bPressEDown = false;
}

void AMainCharacter::LMB()
{
	bLMBDown = true;

	if (EquippedWeapon) {
		Attack();
	}
}

void AMainCharacter::LMBup()
{
	bLMBDown = false;
}

void AMainCharacter::BeginCrouch()
{
	Crouch();
}

void AMainCharacter::EndCrouch()
{
	UnCrouch();
}

void AMainCharacter::OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta,
	const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0 && !bDied) {

		//Die!!
		bDied = true;

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();

		SetLifeSpan(2.f);

	}
}

void AMainCharacter::Attack()
{

	if (!bAttacking) {
		bAttacking = true;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && CombatMontage) {

			AnimInstance->Montage_Play(CombatMontage, 1.0f);
			AnimInstance->Montage_JumpToSection(FName("Attack01"), CombatMontage);
		}
	}
}

void AMainCharacter::AttackEnd()
{
	bAttacking = false;

	if (bLMBDown) {
		Attack();
	}
	else {
		LMBup();
	}
}

