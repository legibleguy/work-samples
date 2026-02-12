// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISMTool.h"
#include "StoreShelfTool.generated.h"

/**
 * 
 */
UCLASS()
class ROLLINPAPERS_API AStoreShelfTool : public AISMTool
{
	GENERATED_BODY()
public:

	AStoreShelfTool();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpaceBetweenRows;

	virtual void SetInitialPresetParams() override;
};
