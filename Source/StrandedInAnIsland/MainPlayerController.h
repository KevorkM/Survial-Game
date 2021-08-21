// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STRANDEDINANISLAND_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
		/** Reference to the UMG asset in the editor */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> HUDOverlayAsset;

	/** Variable to hold the widget after creating it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* HUDOverlay;

protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

};
