// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GroceryActor.h"
#include "Components/StaticMeshComponent.h"
#include "SingleGroceryActor.generated.h"

/**
 * 
 */
UCLASS()
class ROLLINPAPERS_API ASingleGroceryActor : public AGroceryActor
{
	GENERATED_BODY()

public:

	ASingleGroceryActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* ItemSM;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bUsePhysics;

	virtual bool PickUpGrocey(AActor* PickingUpActor, bool DistanceCheck) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void OnConstruction(const FTransform& Transform) override;
	
};
