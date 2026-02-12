// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoInRow_ISM.h"
#include "Kismet/KismetSystemLibrary.h"

int32 UTwoInRow_ISM::getNumberOfOuterSpacing(int32 inX)
{
	int32 counter = -1;
	int32 outNum = 0;
	for (int32 i = 0; i <= inX; i++) {
		if (counter++ == 1) {
			outNum++;
			counter = 0;
		}
	}

	return outNum;
}

FTransform UTwoInRow_ISM::Run()
{
	FTransform NewTr;
	FVector OuterOffset = FVector(SpacingBetweenRows * getNumberOfOuterSpacing(X), 0, 0);
	NewTr.SetLocation(FVector(MeshBounds.X * X, MeshBounds.Y * Y, MeshBounds.Z * Z) + Offset.GetLocation() + FVector(X, Y, Z) * IterativeOffset.GetLocation() + OuterOffset);
	NewTr.SetLocation(NewTr.GetLocation() * Dir);
	NewTr.SetRotation(Offset.GetRotation());
	NewTr.SetScale3D(Offset.GetScale3D());

	return NewTr;
}
