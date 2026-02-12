// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"


// Sets default values
AWeaponActor::AWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TraceRadius = 256.f;
	OwnerIsPlayer = false;
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
    APawn* TargetPawn = Cast<APawn>(GetParentActor());
    if(TargetPawn)UpdateOwner(TargetPawn);
}

void AWeaponActor::SetOwnerIsInRagdoll(bool NewInRagdoll)
{
	OwnerIsInRagdoll = NewInRagdoll;
	OnOwnerRagdollUpdated();
}

void AWeaponActor::OnOwnerRagdollUpdated_Implementation()
{

}

void AWeaponActor::UpdateOwner(APawn* NewPawn){
    OwningPawn = NewPawn;
    OwnerAvailable = OwningPawn != NULL;
    if(OwnerAvailable){
        ACharacter* AsChar = Cast<ACharacter>(OwningPawn);
        if(AsChar) OwningChar = AsChar;
		SetOwner(OwningPawn);

		OwnerIsPlayer = OwningPawn->IsPlayerControlled();
    }
}


void AWeaponActor::Fire_Implementation(bool OverrideEndPoint /*= false*/, FVector CustomEndPoint /*= FVector(0, 0, 0)*/)
{
	bShouldFire = true;
	if (TraceBeforeShooting) PerformMeleeTrace();
	if (!bHoldToShoot) StopFire();
}

void AWeaponActor::StopAim_Implementation()
{

}

TArray<FHitResult> AWeaponActor::PerformMeleeTrace_Implementation()
{
	TArray<FHitResult> Result;
	if (!OwnerAvailable) return Result;
	FVector TraceLoc = OwningChar->GetActorLocation() + OwningChar->GetActorForwardVector() * TraceRadius;
	FVector EndDummy = TraceLoc + OwningChar->GetActorForwardVector();
	TArray<AActor*> Ignore;

	Ignore.Add(OwningChar);
	UKismetSystemLibrary::SphereTraceMulti(this, TraceLoc, EndDummy, TraceRadius, ETraceTypeQuery(), false, Ignore, EDrawDebugTrace::ForDuration, Result, true);
	return Result;
}

void AWeaponActor::StopFire_Implementation()
{
	bShouldFire = false;
}

void AWeaponActor::Aim_Implementation()
{

}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

