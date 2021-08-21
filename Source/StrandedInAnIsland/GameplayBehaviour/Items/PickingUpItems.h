// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickingUpItems.generated.h"

UCLASS()
class STRANDEDINANISLAND_API APickingUpItems : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickingUpItems();

///Variables
public: 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Collision")
		class USphereComponent* CollisionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Items|Mesh")
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sounds")
		class USoundCue* OverlapSound;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

///Functions
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
