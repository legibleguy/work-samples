// Fill out your copyright notice in the Description page of Project Settings.


#include "InstancedGroceryActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CroissantFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"

AInstancedGroceryActor::AInstancedGroceryActor()
{
	ItemISM = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ItemInstancedMesh"));
	ItemISM->SetupAttachment(ItemBounds);
	Availability = 1.f;
	ItemISM->SetCollisionObjectType(ECC_WorldDynamic);
	GroceryTraceDownMode = EGroceryTraceDownType::EGTDT_None;
}

bool AInstancedGroceryActor::PickUpGrocey(AActor* PickingUpActor, bool DistanceCheck)
{
	int32 ShortestIndex = UCroissantFunctionLibrary::GetClosestInstance(ItemISM, PickingUpActor->GetActorLocation(), false);
	FTransform ClosestTr;
	ItemISM->GetInstanceTransform(ShortestIndex, ClosestTr, true);

	if (DistanceCheck) {
		if ((ClosestTr.GetLocation() - PickingUpActor->GetActorLocation()).Size() <= AcceptablePickUpRadius) {
			if (Super::PickUpGrocey(PickingUpActor, DistanceCheck)) {

				TryPlayingPickUpAnimation(PickingUpActor, ClosestTr);

				ItemISM->RemoveInstance(ShortestIndex);
				if (ItemISM->GetInstanceCount() == 0) Destroy();
				return true;
			}
			else return false;
		}
		else return false;
	}
	else {
		if (Super::PickUpGrocey(PickingUpActor, DistanceCheck)) {
			TryPlayingPickUpAnimation(PickingUpActor, ClosestTr);
			Destroy();
			return true;
		}
		else return false;
	}
}

void AInstancedGroceryActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (ItemISM && ItemISM->GetInstanceCount() > 0) ItemISM->ClearInstances();
	if (ThisItemInfo.MeshRef) {

		ItemISM->SetStaticMesh(ThisItemInfo.MeshRef);

		FVector MeshBounds = ThisItemInfo.MeshRef->GetBoundingBox().GetExtent() * 2;
		if (MeshBounds.X < ItemBounds->GetScaledBoxExtent().X * 2 || MeshBounds.Y < ItemBounds->GetScaledBoxExtent().Y * 2 || MeshBounds.Z < ItemBounds->GetScaledBoxExtent().Z * 2) {
			X = FMath::FloorToInt(ItemBounds->GetScaledBoxExtent().X * 2 / MeshBounds.X);
			Y = FMath::FloorToInt(ItemBounds->GetScaledBoxExtent().Y * 2 / MeshBounds.Y);
			Z = FMath::FloorToInt(ItemBounds->GetScaledBoxExtent().Z * 2 / MeshBounds.Z);
			int32 currX, currY, currZ;
			FTransform NewTr;
			FHitResult TraceResult;

			FVector Down = GroceryTraceDownMode == EGroceryTraceDownType::EGTDT_DownVector ? GetActorUpVector() * -1 : FVector(0, 0, -1);

			for (currX = 0; currX < X; currX++) {
				for (currY = 0; currY < Y; currY++) {
					for (currZ = 0; currZ < Z; currZ++) {
						if(!UKismetMathLibrary::RandomBoolWithWeight(Availability)) continue;
						FRotator RandomRot = FRotator(UKismetMathLibrary::RandomFloatInRange(RotMin.Pitch, RotMax.Pitch), UKismetMathLibrary::RandomFloatInRange(RotMin.Yaw, RotMax.Roll), UKismetMathLibrary::RandomFloatInRange(RotMin.Pitch, RotMax.Roll));
						NewTr.SetLocation(FVector(MeshBounds.X * currX, MeshBounds.Y * currY, MeshBounds.Z * currZ) + Offset.GetLocation() - ItemBounds->GetScaledBoxExtent() + MeshBounds/2 + IterativeOffset.GetLocation() * FVector(currX, currY, currZ));
						
						FVector WorldLoc = UKismetMathLibrary::TransformLocation(GetActorTransform(), NewTr.GetLocation());

						if (GroceryTraceDownMode != EGroceryTraceDownType::EGTDT_None) {
							if (UKismetSystemLibrary::LineTraceSingle(this, WorldLoc, WorldLoc + (Down * ItemBounds->Bounds.BoxExtent.Z * 2), TraceTypeQuery1, true, TArray<AActor*>(), EDrawDebugTrace::None, TraceResult, false)) {
								FVector DownLocal = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), Down);
								FVector ImpactLocal = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), TraceResult.ImpactPoint);

								NewTr.SetLocation(ImpactLocal + (Down * -MeshBounds.Z/2));
							}
						}

						NewTr.SetRotation(UKismetMathLibrary::ComposeRotators(RandomRot, Offset.GetRotation().Rotator()).Quaternion());
						NewTr.SetScale3D(Offset.GetScale3D());
						ItemISM->AddInstance(NewTr);

						
					}
				}
			}


			

		}
	}
}
