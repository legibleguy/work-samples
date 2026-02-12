// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GroceryActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "InstancedGroceryActor.generated.h"

UENUM(BlueprintType)
enum class EGroceryTraceDownType : uint8
{
	EGTDT_None     UMETA(DisplayName = "None"),
	EGTDT_NegZ     UMETA(DisplayName = "Negative Z"),
	EGTDT_DownVector     UMETA(DisplayName = "Down Vector")
};

/**
 * 
 */
UCLASS()
class ROLLINPAPERS_API AInstancedGroceryActor : public AGroceryActor
{
	GENERATED_BODY()

public:

	AInstancedGroceryActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInstancedStaticMeshComponent* ItemISM;

	UPROPERTY(EditAnywhere)
		FRotator RotMin;

	UPROPERTY(EditAnywhere)
		FRotator RotMax;

	UPROPERTY(EditAnywhere)
		float Availability;

	UPROPERTY(BlueprintReadOnly)
		int32 X;

	UPROPERTY(BlueprintReadOnly)
		int32 Y;

	UPROPERTY(BlueprintReadOnly)
		int32 Z;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FTransform Offset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FTransform IterativeOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TEnumAsByte<EGroceryTraceDownType> GroceryTraceDownMode;

	virtual bool PickUpGrocey(AActor* PickingUpActor, bool DistanceCheck) override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
};
