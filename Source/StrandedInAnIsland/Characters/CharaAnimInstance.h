// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharaAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class STRANDEDINANISLAND_API UCharaAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = "AnimationProperties")
		void UpdateAnimationProperties();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float movementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		bool bIsInTheAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		class AMainCharacter* Hero;
};
