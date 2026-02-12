// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "GroceriesInventory.h"
#include "CroissantInterface.h"
#include "Components/BoxComponent.h"
#include "GroceryActor.generated.h"

UENUM(BlueprintType)
enum EPickUpPointTraceType
{
	PPTT_X	UMETA(DisplayName = "X"),

	PPTT_Y UMETA(DisplayName = "Y"),

	PPTT_NEGATIVEX UMETA(DisplayName = "-X"),
	
	PPTT_NEGATIVEY UMETA(DisplayName = "-Y"),
	
	PPTT_ALL UMETA(DisplayName = "360 Degrees"),
};

UCLASS()
class ROLLINPAPERS_API AGroceryActor : public AActor, public ICroissantInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGroceryActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Broadcast")
		bool PickUpItem(APawn* NewOnwer);

	virtual bool PickUpItem_Implementation(APawn* NewOwner) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FGroceryItemInfo ThisItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UBoxComponent* ItemBounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AcceptablePickUpRadius;

	UFUNCTION(BlueprintCallable, Category = "Rollin Papers | Groceries")
		virtual bool PickUpGrocey(AActor* PickingUpActor, bool DistanceCheck);

	UFUNCTION(BlueprintPure, Category = "Rollin Papers | Groceries")
		virtual bool ItemDistanceCheck(FVector FromPoint);

	UPROPERTY(BlueprintReadOnly, Category = "Rollin Papers | Groceries")
		TArray<FVector> PickUpPoints;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Rollin Papers | Groceries")
		TEnumAsByte<EPickUpPointTraceType> HowToTracePickUpPoints;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Rollin Papers | Groceries")
		float TraceDistanceMultiply;

	UFUNCTION(BlueprintCallable, Category = "Rollin Papers | Groceries")
		void RegeneratePickUpPoints();

	UFUNCTION(BlueprintPure, Category = "Rollin Papers | Groceries")
		FVector GetClosestPickUpPoint(const FVector& ToPoint) const;

	UFUNCTION()
		void TryPlayingPickUpAnimation(AActor* ForActor, FTransform TargetTransform, bool LeftHand = true);

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:
	UFUNCTION()
		bool PerformTraceForPickUpPoint(const FVector& ForwardVector, const float& Size, FVector& OutResultPoint);
};
