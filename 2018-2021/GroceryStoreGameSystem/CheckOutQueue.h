// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Croissant/Public/CroissantInterface.h"
#include "RollingPapersStuff.h"
#include "GameFramework/Character.h"
#include "CheckOutQueue.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQueueEndUpdated, FVector, NewLocation);

UCLASS()
class ROLLINPAPERS_API ACheckOutQueue : public AActor, public ICroissantInterface, public IRollingPapersStuff
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckOutQueue();

	//Will be called every time somebody enters/leaves the line
	UPROPERTY(BlueprintAssignable)
		FQueueEndUpdated LineWasUpdated;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		USplineComponent* QueueSpline;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool ReverseQueuePosition;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		USplineComponent* CheckoutSpline;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		UBoxComponent* EntryBox;

	//Will be increased when a person joins in the line
	UPROPERTY(BlueprintReadWrite)
		float UsedLenght;

	UPROPERTY(BlueprintReadOnly)
		float DefaultCharacterSize;

	UPROPERTY(BlueprintReadOnly)
		int32 NumCharactersToTrace;

	UPROPERTY(BlueprintReadOnly)
		APlayerController* LastOverlappedPC;

	UPROPERTY(BlueprintReadOnly)
		TArray<ACharacter*> CharactersInTheQueue;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		ACharacter* CashierRef;

	UPROPERTY()
		FTransform SecondToLastCharTransform;

	UFUNCTION()
		bool TraceCharactersInQueue();

	UFUNCTION()
		void AllCharactersGoToPositions();

	UFUNCTION()
		bool AreAllCharactersInPosition();

	UPROPERTY()
		bool bWaitForCharactersToGoToPosition;

	UPROPERTY()
		bool bWaitForCheckOut;

	UFUNCTION(BlueprintPure)
		FVector GetCurrentEnterLocation() const;

	UFUNCTION()
		void UpdateEntryBoxPosition();

	UFUNCTION(BlueprintCallable)
		void AddPawnToQueue(ACharacter* PawnToAdd);

	UFUNCTION()
		void EntryOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		
	UFUNCTION()
		void EntryEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Will be used for interaction with AI in this context
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Broadcast")
		bool PickUpItem(APawn* NewOnwer);

	virtual bool PickUpItem_Implementation(APawn* NewOwner) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Broadcast")
		void CheckOutFinished();
	virtual void CheckOutFinished_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
