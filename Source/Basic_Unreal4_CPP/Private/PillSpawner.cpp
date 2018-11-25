// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/PillSpawner.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
APillSpawner::APillSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningVolumeBox"));
	RootComponent = SpawningVolume;
}

// Called when the game starts or when spawned
void APillSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APillSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

