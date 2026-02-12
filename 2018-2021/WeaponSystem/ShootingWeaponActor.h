// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "ShootingWeaponActor.generated.h"

UENUM(BlueprintType)
enum class EFiringMode : uint8
{
	ESM_Tracing     UMETA(DisplayName = "Tracing"),
	ESM_True     UMETA(DisplayName = "True")
};

/**
 * 
 */
UCLASS()
class CROISSANTWEAPON_API AShootingWeaponActor : public AWeaponActor
{
	GENERATED_BODY()
public:

	
};
