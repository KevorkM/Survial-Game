// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

///Enums
UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Dead UMETA(DisplayName = "Dead"),

	EMS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EStaminaStatus :uint8
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum UMETA(DisplayName = "BelowMinimum"),
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering"),

	ESS_MAX UMETA(DisplayName = "DefaultMax")

};

UCLASS()
class STRANDEDINANISLAND_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()
 
public:

	// Sets default values for this character's properties
	AMainCharacter();

	//Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent*CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent*SpringArmComp;

	//Getter for the camera
	FORCEINLINE UCameraComponent*GetCameraComponent() { return CameraComp; }
	FORCEINLINE void SetCameraComponent(UCameraComponent*InCamera) { CameraComp = InCamera; }

	FORCEINLINE USpringArmComponent*GetSpringArmComponent() { return SpringArmComp; }
	FORCEINLINE void SetSpringArmComponent(USpringArmComponent*InSpringArm) { SpringArmComp = InSpringArm; }

	//Health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UHealthComponent* HealthComp;

	//character movement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	///Player Sprinting Status
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
		float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
		float SprintingSpeed;

	//Boolean
	bool bMovingForward;
	bool bMovingRight;
	bool bShiftKeyDown;
	bool bPressEDown = false;
	/* Pawn died previously */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	bool bDied;

	///Enum variables
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EMovementStatus MovementStatus;

	///State values
	//Stamina
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus StaminaStatus;

	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float MinSprintStamina;

	//Character Stats
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
		float MaxStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stats’")
		float Stamina;

	//Hunger

	//Hydration

	//Equipping
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
		class AMeleeWeapon* EquippedWeapon;

	FORCEINLINE void SetEquippedWeapon(AMeleeWeapon* WeaponToSet) { EquippedWeapon = WeaponToSet; }


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
		class APickingUpItems* ActiveOverlappingItem;

	FORCEINLINE void SetActiveOverlappingItem(APickingUpItems* Item) { ActiveOverlappingItem = Item; }

	//Attacking
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim")
	bool bAttacking;

	bool bLMBDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	///Character movement
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetMovementStatus(EMovementStatus Status);

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void Turn(float Value);

	void LookUp(float Value);

	void ShiftKeyDown();

	void ShiftKeyUp();

	void PressEDown();

	void PressEUp();

	void LMB();

	void LMBup();

	UFUNCTION()
	void BeginCrouch();

	UFUNCTION()
	void EndCrouch();

	UFUNCTION()
	void OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta,
		const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void Attack();

	UFUNCTION(BlueprintCallable)
		void AttackEnd();

};
