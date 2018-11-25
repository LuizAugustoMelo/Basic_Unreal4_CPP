// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PillSpawner.generated.h"

UCLASS()
class BASIC_UNREAL4_CPP_API APillSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APillSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//This is the Item to Spawn
	UPROPERTY(EditAnyWhere, Category="Spwaning")
		TSubclassOf<class AMagicPill> ItemToSpwan;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//This is the Spawning Volume as a Box Component
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category="Spawning", meta=(AllowPrivateAccess="true"))
		class UBoxComponent* SpawningVolume;
};
