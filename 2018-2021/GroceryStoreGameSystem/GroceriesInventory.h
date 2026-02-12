// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/DataTable.h"
#include "GroceriesInventory.generated.h"

USTRUCT(BlueprintType)
struct FGroceryItemInfo : public FTableRowBase
{
	GENERATED_BODY()

		FGroceryItemInfo() {};

	FGroceryItemInfo(const FGroceryItemInfo* OtherItem) { MeshRef = OtherItem->MeshRef; ItemName = OtherItem->ItemName; ItemPriority = OtherItem->ItemPriority; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
		UStaticMesh* MeshRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		int32 ItemPriority;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		float Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		bool PricePerLb;

	/* Operators */
	FORCEINLINE bool operator==(const FGroceryItemInfo& Right) const;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryUpdated, TArray<FGroceryItemInfo>, Items);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROLLINPAPERS_API UGroceriesInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGroceriesInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
		TArray<FGroceryItemInfo> Items;

	UPROPERTY()
		FInventoryUpdated OnInventoryUpdated;

	UPROPERTY()
		TArray<UGroceriesInventory*> SubInventories;

	UPROPERTY()
		TArray<int32> ItemsAmounts;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector MaxInventoryExt;

	UFUNCTION(BlueprintCallable, Category = "Rollin Papers | Groceries")
		bool AddItem(const FGroceryItemInfo& ItemToAdd, int32 NumToAdd = 1, bool AddToTheBiggestInvFirst = true);

	UFUNCTION(BlueprintCallable, Category = "Rollin Papers | Groceries")
		void RemoveItem(const FGroceryItemInfo& ItemToRemove, int32 NumToRemove = 1, bool RemoveFromTheBiggestInvFirst = true);

	UFUNCTION(BlueprintCallable, Category = "Rollin Papers | Groceries")
		void RemoveItemsOfType(const FGroceryItemInfo& ItemsToRemove, bool IncludeSubInventories = true);

	UFUNCTION(BlueprintCallable, Category = "Rollin Papers | Groceries")
		void ClearInventory(bool IncludeSubInventories = true);

	UFUNCTION(BlueprintPure, Category = "Rollin Papers | Groceries")
		UGroceriesInventory* GetTheBiggestSubInventory();

	UFUNCTION(BlueprintPure, Category = "Rollin Papers | Groceries")
		float GetOccupiedSpace();

	UFUNCTION(BlueprintPure, Category = "Rollin Papers | Groceries")
		FORCEINLINE float GetOccupiedSpaceFraction() { return GetOccupiedSpace() / MaxInventoryExt.Size(); }

	UFUNCTION(BlueprintPure, Category = "Rollin Papers | Groceries")
		bool IsEnoughSpaceoOrItem(const FGroceryItemInfo& InItem);

	UFUNCTION(BlueprintPure, Category = "Rollin Papers | Groceries")
		int32 GetTotalAmountOfGroceries() const;
};

FORCEINLINE bool FGroceryItemInfo::operator==(const FGroceryItemInfo& Right) const
{
	return (ItemName == Right.ItemName && ItemPriority == Right.ItemPriority);
}
