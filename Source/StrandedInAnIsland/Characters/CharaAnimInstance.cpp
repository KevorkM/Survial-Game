// Fill out your copyright notice in the Description page of Project Settings.


#include "CharaAnimInstance.h"
#include "Characters/MainCharacter.h"

#include <GameFramework/CharacterMovementComponent.h>

void UCharaAnimInstance::NativeInitializeAnimation() {
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn) {
			Hero = Cast<AMainCharacter>(Pawn);
		}
	}
}

void UCharaAnimInstance::UpdateAnimationProperties()
{

	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
	}
	if (Pawn) {
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		movementSpeed = LateralSpeed.Size();

		bIsInTheAir = Pawn->GetMovementComponent()->IsFalling();

		if (Hero == nullptr) {
			Hero = Cast<AMainCharacter>(Pawn);
		}


	}

}
