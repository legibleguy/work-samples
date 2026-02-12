// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISMTool_Preset.h"
#include "TwoInRow_ISM.generated.h"

/**
 * 
 */
UCLASS()
class ROLLINPAPERS_API UTwoInRow_ISM : public UISMTool_Preset
{
	GENERATED_BODY()

public:

	UPROPERTY()
		float SpacingBetweenRows;

	UPROPERTY()
		float SpacingInsideRow;

	int32 getNumberOfOuterSpacing(int32 inX);

	virtual FTransform Run() override;
	
};
