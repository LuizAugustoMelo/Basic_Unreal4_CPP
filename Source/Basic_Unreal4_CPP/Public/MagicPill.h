// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicPill.generated.h"

UCLASS()
class BASIC_UNREAL4_CPP_API AMagicPill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagicPill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//My Added items to the class
protected:
	// The amount of health that the pill has (+/-)
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category="Values")
		float PillEffect;

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category="Components")
		UStaticMeshComponent* PillMesh;
};
