// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponActor.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "WeaponManager.generated.h"

USTRUCT(BlueprintType)
struct FWeaponInfo
{
    GENERATED_USTRUCT_BODY();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
    FName CustomName;

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<class UAnimInstance> AnimInstRef;
    */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FName AnimLayerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
    TSubclassOf<class AWeaponActor> WeaponClass;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
    int32 Ammo;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FWeaponChangedSignature, int32, PrevWeapon, int32, NewWeapon, bool, LeftHandChange);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CROISSANTWEAPON_API UWeaponManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponManager();

	UPROPERTY(BlueprintAssignable, Category = "Weapon Manager")
		FWeaponChangedSignature WeaponChanged;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Manager")
    bool bDestroyUnusedWeapon;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Owner")
    FName RightHandSocket;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Owner")
    FName LeftHandSocket;
    
    UPROPERTY(BlueprintReadWrite, Category = "Weapon Manager")
    AWeaponActor* CurrentWeaponRight;
    
    UPROPERTY(BlueprintReadOnly, Category = "Weapon Manager")
    int32 CurrentWeaponRightIndex;
    
    UPROPERTY(BlueprintReadWrite, Category = "Weapon Manager")
    AWeaponActor* CurrentWeaponLeft;
    
    UPROPERTY(BlueprintReadOnly, Category = "Weapon Manager")
    int32 CurrentWeaponLeftIndex;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Manager")
    TArray<FWeaponInfo> WeaponsInventory;
    
    UFUNCTION(BlueprintCallable, Category = "Weapon Manager")
		void GetWeaponWithIndex(int32 InIndex, bool LeftHand = false);

	UFUNCTION(BlueprintCallable, Category = "Weapon Manager")
		bool GetWeaponOfClass(TSubclassOf<class AWeaponActor> WeaponClass, bool LeftHand = false);

	UFUNCTION(BlueprintCallable, Category = "Weapon Manager")
		bool GetWeaponByName(FName InName, bool LeftHand = false);

	UFUNCTION(BlueprintCallable, Category = "Weapon Manager")
		void GetNextWeapon(bool LeftHand = false);
	
	UFUNCTION(BlueprintCallable, Category = "Weapon Manager")
		void GetPrevWeapon(bool LeftHand = false);
    
    UPROPERTY(BlueprintReadOnly, Category = "Owner")
    ACharacter* Char;
    
    UPROPERTY(BlueprintReadOnly, Category = "Owner")
    USkeletalMeshComponent* OwnerMesh;
    

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
