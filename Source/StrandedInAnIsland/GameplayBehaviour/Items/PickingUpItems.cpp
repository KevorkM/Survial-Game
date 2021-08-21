// Fill out your copyright notice in the Description page of Project Settings.


#include "PickingUpItems.h"

#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Engine/StaticMesh.h>


// Sets default values
APickingUpItems::APickingUpItems()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	RootComponent = CollisionVolume;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());


}

// Called when the game starts or when spawned
void APickingUpItems::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &APickingUpItems::OnOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &APickingUpItems::OnOverlapEnd);

}

// Called every frame
void APickingUpItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickingUpItems::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}

void APickingUpItems::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
}