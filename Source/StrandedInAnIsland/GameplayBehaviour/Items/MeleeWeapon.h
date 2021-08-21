// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayBehaviour/Items/PickingUpItems.h"
#include "MeleeWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState:uint8 {

	EWS_Pickup		UMETA(DisplayName = "Pickup"),
	EWS_Equiped		UMETA(DisplayName = "Equiped"),

	EWS_MAX			UMETA(DisplayName = "Default")
};

UCLASS()
class STRANDEDINANISLAND_API AMeleeWeapon : public APickingUpItems
{
	GENERATED_BODY()

		AMeleeWeapon();
		

	//Variables
public:

	//Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Items|Mesh")
		class USkeletalMeshComponent* SkeletalMesh;

	 //Sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
		class USoundCue* SwingSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
		class USoundCue* OnEquipSound;

	//Enums variables
	EWeaponState WeaponState;

	FORCEINLINE void SetWeaponState(EWeaponState State) { WeaponState = State; }
	FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }

protected:
	

	//Functions
public:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	void Equip(class AMainCharacter* Char);

};
