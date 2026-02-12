// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "ThrowPreview.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = ("Croissant"), meta = (BlueprintSpawnableComponent))
class CROISSANTWEAPON_API UThrowPreview : public UInstancedStaticMeshComponent
{
	GENERATED_BODY()
	
public:

	UThrowPreview();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float DistanceBtwnInstances;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FVector LaunchVelocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float TraceRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray <TEnumAsByte<EObjectTypeQuery>> ObjectsToTrace;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
