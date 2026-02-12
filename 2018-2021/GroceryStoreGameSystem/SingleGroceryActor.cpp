// Fill out your copyright notice in the Description page of Project Settings.


#include "SingleGroceryActor.h"

ASingleGroceryActor::ASingleGroceryActor() {
	ItemSM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMesh"));
	ItemSM->SetupAttachment(ItemBounds);
	ItemSM->SetCollisionObjectType(ECC_WorldDynamic);
}

bool ASingleGroceryActor::PickUpGrocey(AActor* PickingUpActor, bool DistanceCheck)
{
	if (DistanceCheck) {
		FVector ClosestPoint;
		ItemSM->GetClosestPointOnCollision(PickingUpActor->GetActorLocation(), ClosestPoint);
		if ((ClosestPoint - PickingUpActor->GetActorLocation()).Size() <= AcceptablePickUpRadius) {
			if (Super::PickUpGrocey(PickingUpActor, DistanceCheck)) {
				TryPlayingPickUpAnimation(PickingUpActor, GetActorTransform());
				Destroy();
				return true;
			}
			else return false;
		}
		else return false;
	}
	else {
		if (Super::PickUpGrocey(PickingUpActor, DistanceCheck)) {
			Destroy();
			return true;
		}
		else return false;
	}
}

void ASingleGroceryActor::BeginPlay()
{
	Super::BeginPlay();
	if(ItemSM && ItemSM->GetStaticMesh())ItemSM->SetSimulatePhysics(bUsePhysics);
}

void ASingleGroceryActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ItemSM->SetStaticMesh(ThisItemInfo.MeshRef);
}

