// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowPreview.h"
#include "Kismet/GameplayStatics.h"

UThrowPreview::UThrowPreview()
{
	DistanceBtwnInstances = 20;
	TraceRadius = 10;
	SetComponentTickEnabled(true);
}

void UThrowPreview::BeginPlay()
{
	Super::BeginPlay();
}

void UThrowPreview::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FHitResult PredictHit;
	TArray<FVector> Path;
	FVector TargetLoc;
	
	//UGameplayStatics::PredictProjectilePath(this, PredictHit, Path, TargetLoc, GetComponentLocation(), LaunchVelocity, true, TraceRadius, ObjectsToTrace, false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, 1.f, 3.f, 20.f);
}