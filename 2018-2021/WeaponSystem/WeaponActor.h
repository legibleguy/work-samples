// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "WeaponActor.generated.h"

UCLASS()
class CROISSANTWEAPON_API AWeaponActor : public ASkeletalMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponActor();
    
    UPROPERTY(BlueprintReadOnly, Category = "Character")
    APawn* OwningPawn;
    
    UPROPERTY(BlueprintReadOnly, Category = "Character")
    ACharacter* OwningChar;

	UFUNCTION(BlueprintPure, Category = "Character")
		FORCEINLINE ACharacter* GetOwningChar() { return OwningChar; }

	UFUNCTION(BlueprintPure, Category = "Character")
		FORCEINLINE APawn* GetOwningPawn() { return OwningPawn; }
    
    UPROPERTY(BlueprintReadOnly, Category = "Character")
    bool OwnerAvailable;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
		bool OwnerIsPlayer;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
		bool OwnerIsInRagdoll;

	UFUNCTION(BlueprintCallable, Category = "Character")
		void SetOwnerIsInRagdoll(bool NewInRagdoll);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent,  Category = "Weapon")
		void TryToSetCollisionEnabled(bool NewEnabled);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character")
		void OnOwnerRagdollUpdated();
		void OnOwnerRagdollUpdated_Implementation();
    
    UFUNCTION(BlueprintCallable, Category = "Character")
    void UpdateOwner(APawn* NewPawn);
 
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Attachment")
    FTransform ForLeftHand;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Attachment")
    FTransform ForRightHand;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Attachment")
    bool IsInRightHand;
     
    UPROPERTY(BlueprintReadWrite, Category = "Firing")
    TArray<AActor*> IgnoreActors;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		FName WeaponTypeName;

	UFUNCTION(BlueprintPure, Category = "Weapons")
		FORCEINLINE FName GetWeaponTypeName() { return WeaponTypeName; };

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon")
		bool bHoldToShoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Animation")
		FTransform SecondHandTr;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		bool bShouldFire;
    
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Shooting")
		void Fire(bool OverrideEndPoint = false, FVector CustomEndPoint = FVector(0, 0, 0));
		void Fire_Implementation(bool OverrideEndPoint = false, FVector CustomEndPoint = FVector(0, 0, 0));

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Shooting")
		void StopFire();
		void StopFire_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Shooting")
		void Aim();
		void Aim_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Shooting")
		void StopAim();
		void StopAim_Implementation();

	UPROPERTY(EditDefaultsOnly, Category = "Melee Trace")
		bool TraceBeforeShooting;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee Trace")
		float TraceRadius;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Melee Trace")
		TArray<FHitResult> PerformMeleeTrace();

	TArray<FHitResult> PerformMeleeTrace_Implementation();

	UPROPERTY(BlueprintReadOnly, Category = "Melee")
		bool IsPlayingMeleeAttack;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
