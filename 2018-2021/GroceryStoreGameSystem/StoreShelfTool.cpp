// Fill out your copyright notice in the Description page of Project Settings.


#include "StoreShelfTool.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TwoInRow_ISM.h"

AStoreShelfTool::AStoreShelfTool()
{
	SpaceBetweenRows = 150.0;
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/MarketplaceContent/ResidentialGarage/Meshes/Props/Static_Meshes/SM_Shelves_01.SM_Shelves_01"));
	if (MeshObj.Object) BaseMesh = MeshObj.Object;

	const ConstructorHelpers::FObjectFinder<UClass> LogicObj(TEXT("/Script/RollinPapers.TwoInRow_ISM"));
	if (LogicObj.Object) InstanceLogic = LogicObj.Object;
}

void AStoreShelfTool::SetInitialPresetParams()
{
	Super::SetInitialPresetParams();

	auto asTIR = Cast<UTwoInRow_ISM>(LogicPreset);

	if (asTIR) asTIR->SpacingBetweenRows = SpaceBetweenRows;
}
