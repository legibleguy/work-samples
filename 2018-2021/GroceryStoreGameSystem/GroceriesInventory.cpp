// Fill out your copyright notice in the Description page of Project Settings.


#include "GroceriesInventory.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/StaticMesh.h"

// Sets default values for this component's properties
UGroceriesInventory::UGroceriesInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	MaxInventoryExt = FVector(1, 1, 1);

	// ...
}


// Called when the game starts
void UGroceriesInventory::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


bool UGroceriesInventory::AddItem(const FGroceryItemInfo& ItemToAdd, int32 NumToAdd/* = 1*/, bool AddToTheBiggestInvFirst /*= true*/)
{
	UGroceriesInventory* TargetInv = AddToTheBiggestInvFirst ? GetTheBiggestSubInventory() : this;

	if (TargetInv->IsEnoughSpaceoOrItem(ItemToAdd)) {
		if (TargetInv->Items.Contains(ItemToAdd)) TargetInv->ItemsAmounts[TargetInv->Items.Find(ItemToAdd)] += NumToAdd;
		else {
			TargetInv->Items.Add(ItemToAdd);
			TargetInv->ItemsAmounts.Add(NumToAdd);
		}
		if (TargetInv->OnInventoryUpdated.IsBound()) {
			TargetInv->OnInventoryUpdated.Broadcast(TargetInv->Items);
		}
		return true;
	}
	else return false;
}

void UGroceriesInventory::RemoveItem(const FGroceryItemInfo& ItemToRemove, int32 NumToRemove /*= 1*/, bool RemoveFromTheBiggestInvFirst/* = true*/)
{
	UGroceriesInventory* TargetInv = RemoveFromTheBiggestInvFirst ? GetTheBiggestSubInventory() : this;

	int32 i = TargetInv->Items.Find(ItemToRemove);
	if (i != -1) {
		if (TargetInv->ItemsAmounts[i] - NumToRemove > 0) TargetInv->ItemsAmounts[i] -= NumToRemove;
		else {
			TargetInv->Items.RemoveAt(i);
			TargetInv->ItemsAmounts.RemoveAt(i);
		}
		if (TargetInv->OnInventoryUpdated.IsBound()) {
			TargetInv->OnInventoryUpdated.Broadcast(TargetInv->Items);
		}
	}
}

void UGroceriesInventory::RemoveItemsOfType(const FGroceryItemInfo& ItemsToRemove, bool IncludeSubInventories/* = true*/)
{

	int32 i = Items.Find(ItemsToRemove);
	if (i != -1) {
		Items.RemoveAt(i);
		ItemsAmounts.RemoveAt(i);
		if (OnInventoryUpdated.IsBound()) {
			OnInventoryUpdated.Broadcast(Items);
		}
	}
	for (int32 g = 0; g < SubInventories.Num(); g++) SubInventories[g]->RemoveItemsOfType(ItemsToRemove, true);
}

void UGroceriesInventory::ClearInventory(bool IncludeSubInventories /*= true*/)
{
	Items.Empty();
	ItemsAmounts.Empty();
	if (OnInventoryUpdated.IsBound()) {
		OnInventoryUpdated.Broadcast(Items);
	}
	for (int32 i = 0; i < SubInventories.Num(); i++) SubInventories[i]->ClearInventory();
}

UGroceriesInventory* UGroceriesInventory::GetTheBiggestSubInventory()
{
	UGroceriesInventory* TargetInv = this;
	float CurrBiggestSize = 0;
	float CurrSize;
	int32 CurrBiggestSizeIndex = -1;


	for (int32 i = 0; i < SubInventories.Num(); i++) {
		if (SubInventories[i])CurrSize = SubInventories[i]->MaxInventoryExt.Size();
		else continue;

		if (CurrSize > CurrBiggestSize && CurrSize > MaxInventoryExt.Size()) {
			CurrBiggestSize = CurrSize;
			CurrBiggestSizeIndex = i;
		}
	}
	if (CurrBiggestSizeIndex != -1 && SubInventories[CurrBiggestSizeIndex]) TargetInv = SubInventories[CurrBiggestSizeIndex];
	return TargetInv;
}

float UGroceriesInventory::GetOccupiedSpace()
{
	float Total = 0;
	for (int32 i = 0; i < Items.Num(); i++) {
		UStaticMesh* CurrMesh = Items[i].MeshRef;
		if (CurrMesh) Total += (CurrMesh->GetBoundingBox().GetExtent() * 2).Size();
	}
	return Total;
}

bool UGroceriesInventory::IsEnoughSpaceoOrItem(const FGroceryItemInfo& InItem)
{
	float ThisSize = 0;
	if (InItem.MeshRef) ThisSize = (InItem.MeshRef->GetBoundingBox().GetExtent() * 2).Size();
	//UKismetSystemLibrary::PrintString(this, "Will be occupied: " + FString::SanitizeFloat(GetOccupiedSpace() + ThisSize));
	//UKismetSystemLibrary::PrintString(this, "And the maximum is: " + FString::SanitizeFloat(MaxInventoryExt.Size()));

	return (GetOccupiedSpace() + ThisSize) <= MaxInventoryExt.Size();
}

int32 UGroceriesInventory::GetTotalAmountOfGroceries() const
{
	int32 Total = 0;
	for (int32 i = 0; i < ItemsAmounts.Num(); i++) Total += ItemsAmounts[i];
	for (int32 i = 0; i < SubInventories.Num(); i++) if (SubInventories[i]) Total += SubInventories[i]->GetTotalAmountOfGroceries();
	return Total;
}

// Called every frame
void UGroceriesInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

