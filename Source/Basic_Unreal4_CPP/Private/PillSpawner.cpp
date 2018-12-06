// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/PillSpawner.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Public/MagicPill.h"
#include "Classes/Kismet/KismetMathLibrary.h"

// Sets default values
APillSpawner::APillSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningVolumeBox"));
	RootComponent = SpawningVolume;

	//This establishes the item to spwan is a magic pill
	//ItemToSpwan = AMagicPill::StaticClass();
}

// Called when the game starts or when spawned
void APillSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnPills();
}

// Called every frame
void APillSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// This is the function that returns a random point in the volume
FVector APillSpawner::GetRandomPointInVolume()
{
	FVector Origin = SpawningVolume->Bounds.Origin;
	FVector Extent = SpawningVolume->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
}

//This function spawns magic Pills
void APillSpawner::SpawnPills()
{
	if (ItemToSpwan != NULL)
	{
		//This variable get the World to spawn pills
		UWorld* const World = GetWorld();

		//Verify if world not is null
		if (World)
		{
			//This variable contains the Random Point create using the Spawning Valume
			FVector SpawnLocation = GetRandomPointInVolume();

			//This variable create a random rotation to the pill
			FRotator SpawnRotation;
			SpawnRotation.Pitch = FMath::Rand()*360.0f;
			SpawnRotation.Roll = FMath::Rand()*360.0f;
			SpawnRotation.Yaw = FMath::Rand()*360.0f;

			AMagicPill* SpawnedPill = World->SpawnActor<AMagicPill>(ItemToSpwan, SpawnLocation, SpawnRotation);
		}
	}
}

