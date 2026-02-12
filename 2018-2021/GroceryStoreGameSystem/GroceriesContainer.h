// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GroceriesInventory.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "CroissantInterface.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GameFramework/Character.h"
#include "GroceriesContainer.generated.h"

UENUM(BlueprintType)
enum EShoppingContainerType
{
	SCT_NONE	UMETA(DisplayName = "None"),

	SCT_BASKET UMETA(DisplayName = "Basket"),

	SCT_CART UMETA(DisplayName = "Cart"),
};

UENUM(BlueprintType)
enum EBasketAttachementType
{
	BAT_HAND_R	UMETA(DisplayName = "Right Hand"),

	BAT_HAND_L UMETA(DisplayName = "Left Hand"),

	BAT_ELBOW_R UMETA(DisplayName = "Right Elbow"),

	BAT_ELBOW_L UMETA(DisplayName = "Left Elbow"),
};

UCLASS()
class ROLLINPAPERS_API AGroceriesContainer : public AActor, public ICroissantInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGroceriesContainer();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Broadcast")
		bool PickUpItem(APawn* NewOwner);
	virtual bool PickUpItem_Implementation(APawn* NewOwner) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Broadcast")
		void DropItem();
	virtual void DropItem_Implementation() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector Offset;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		bool UsePhysicsOnGrab;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator RotationOffset;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UBoxComponent* Extent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UBoxComponent* BottomBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AvailabilityMultiply;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UBoxComponent* EnterBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UGroceriesInventory* Inventory;

	UPROPERTY(BlueprintReadOnly)
		UGroceriesInventory* CurrOwnerInv;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		UStaticMeshComponent* MainMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		UPhysicsHandleComponent* Handle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName GrabSocket;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		FText InteractionText;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		TEnumAsByte<EShoppingContainerType> ContainerType;

	UFUNCTION(BlueprintCallable, Category = "Rolling Papers | Groceries | Shopping Cart")
		void SetOffset(const FVector NewOffset);

	UFUNCTION(BlueprintCallable, Category = "Rolling Papers | Groceries | Shopping Cart")
		void SetRotationOffset(const FRotator NewOffset);

	UFUNCTION(BlueprintCallable, Category = "Rolling Papers | Groceries | Shopping Cart")
		void GrabCart(ACharacter* NewOwner, bool bUsePhysics = true);

	UFUNCTION(BlueprintCallable, Category = "Rolling Papers | Groceries | Shopping Cart")
		void ReleaseCart();

	UFUNCTION(BlueprintCallable, Category = "Rolling Papers | Groceries | Shopping Cart")
		void UpdateGroceryContents(TArray<FGroceryItemInfo> CurrentItems);

	UFUNCTION(BlueprintCallable, Category = "Rolling Papers | Groceries | Shopping Cart")
		void ChangeAttachSocket(const FName NewAttachSocket);

	UPROPERTY(BlueprintReadOnly, Category = "Rolling Papers | Groceries | Shopping Cart")
		TArray<UInstancedStaticMeshComponent*> Groceries;

	UFUNCTION()
		void RemoveMessage(AActor* FromActor);

	UPROPERTY(EditDefaultsOnly)
		bool bFreeRotationDuringPhysics;

	UPROPERTY(BlueprintReadOnly)
		ACharacter* CurrOwner;

	UPROPERTY(BlueprintReadOnly)
		APlayerController* LastPC;

	UPROPERTY(BlueprintReadOnly)
		FName CurrAttachSocket;

	UFUNCTION()
		void EntranceOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void EntranceEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
