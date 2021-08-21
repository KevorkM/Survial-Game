// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "GameplayBehaviour/Items/PickingUpItems.h"
#include "Characters/MainCharacter.h"

#include <Components/SKeletalMeshComponent.h>
#include <Engine/SkeletalMeshSocket.h>
#include <Kismet/GameplayStatics.h>

AMeleeWeapon::AMeleeWeapon()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	EWeaponState::EWS_Pickup;
}

void AMeleeWeapon::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if ((WeaponState==EWeaponState::EWS_Pickup)&& OtherActor) {
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);

		if (Main) {
			Main->SetActiveOverlappingItem(this);
		}
	}

}

void AMeleeWeapon::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor) {
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main) {
			Main->SetActiveOverlappingItem(nullptr);
		}
	}
}

void AMeleeWeapon::Equip(AMainCharacter* Char)
{
	if (Char) {

//		SetInstigator(Char->GetController());

		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		WeaponState = EWeaponState::EWS_Equiped;

		SkeletalMesh->SetSimulatePhysics(false);

		const USkeletalMeshSocket* RightHandSocket = Char->GetMesh()->GetSocketByName("RightHandSocket");

		if (RightHandSocket) {
			RightHandSocket->AttachActor(this, Char->GetMesh());

			Char->SetEquippedWeapon(this);
			Char->SetActiveOverlappingItem(nullptr);
		}
		//if (OnEquipSound) UGameplayStatics::PlaySound2D(this, OnEquipSound);
	}
}