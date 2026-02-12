// Fill out your copyright notice in the Description page of Project Settings.


#include "GroceryActor.h"
#include "Croissant/Public/CroissantFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AGroceryActor::AGroceryActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemBoundBox"));
	SetRootComponent(ItemBounds);
	HowToTracePickUpPoints = EPickUpPointTraceType::PPTT_Y;
	TraceDistanceMultiply = 75.f;
	AcceptablePickUpRadius = 200.f;
}

// Called when the game starts or when spawned
void AGroceryActor::BeginPlay()
{
	Super::BeginPlay();
	RegeneratePickUpPoints();
	
	ItemBounds->OnComponentBeginOverlap.AddDynamic(this, &AGroceryActor::OnOverlap);
	
}

// Called every frame
void AGroceryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGroceryActor::PickUpItem_Implementation(APawn* NewOwner)
{
	return PickUpGrocey(NewOwner, true);
}

bool AGroceryActor::PickUpGrocey(AActor* PickingUpActor, bool DistanceCheck)
{
	auto MaybeInventory = PickingUpActor->GetComponentByClass(UGroceriesInventory::StaticClass());
	if (MaybeInventory) {
		UGroceriesInventory* Inventory = Cast<UGroceriesInventory>(MaybeInventory);
		if (Inventory) return Inventory->AddItem(ThisItemInfo);
		else return false;
	}
	else return false;
}

bool AGroceryActor::ItemDistanceCheck(FVector FromPoint)
{
	return true;
}

bool AGroceryActor::PerformTraceForPickUpPoint(const FVector& ForwardVector, const float& Size, FVector& OutResultPoint) {
	FHitResult OutHit;
	FVector TraceStart = GetActorLocation() + (ForwardVector * TraceDistanceMultiply);
	FVector TraceEnd = FVector(TraceStart.X, TraceStart.Y, -10000);
	bool Result = (UKismetSystemLibrary::LineTraceSingle(this, TraceStart, TraceEnd, TraceTypeQuery1, false, TArray<AActor*>(), EDrawDebugTrace::None, OutHit, true));
	if (Result) OutResultPoint = OutHit.ImpactPoint;
	return Result;
}

void AGroceryActor::RegeneratePickUpPoints()
{
	PickUpPoints.Reset();

	FHitResult OutHit;
	FVector TraceStart, TraceEnd, vBounds, vExtent, HitPoint;
	float SizeX, SizeY;
	vExtent = ItemBounds->GetScaledBoxExtent();
	SizeX = vExtent.X;
	SizeY = vExtent.Y;

	if (HowToTracePickUpPoints == EPickUpPointTraceType::PPTT_X) {
		if (PerformTraceForPickUpPoint(GetActorForwardVector(), SizeX, HitPoint)) PickUpPoints.Add(HitPoint);
	}
	else if (HowToTracePickUpPoints == EPickUpPointTraceType::PPTT_NEGATIVEX) {
		if (PerformTraceForPickUpPoint(-GetActorForwardVector(), -SizeX, HitPoint)) PickUpPoints.Add(HitPoint);
	}
	else if (HowToTracePickUpPoints == EPickUpPointTraceType::PPTT_Y) {
		if (PerformTraceForPickUpPoint(GetActorRightVector(), SizeY, HitPoint)) PickUpPoints.Add(HitPoint);
	}
	else if (HowToTracePickUpPoints == EPickUpPointTraceType::PPTT_NEGATIVEY) {
		if (PerformTraceForPickUpPoint(-GetActorRightVector(), -SizeY, HitPoint)) PickUpPoints.Add(HitPoint);
	}
	else {
		if (PerformTraceForPickUpPoint(GetActorForwardVector(), SizeX, HitPoint)) PickUpPoints.Add(HitPoint);
		if (PerformTraceForPickUpPoint(-GetActorForwardVector(), -SizeX, HitPoint)) PickUpPoints.Add(HitPoint);
		if (PerformTraceForPickUpPoint(GetActorRightVector(), SizeY, HitPoint)) PickUpPoints.Add(HitPoint);
		if (PerformTraceForPickUpPoint(-GetActorRightVector(), -SizeY, HitPoint)) PickUpPoints.Add(HitPoint);
	}
}

FVector AGroceryActor::GetClosestPickUpPoint(const FVector& ToPoint) const
{
	if (PickUpPoints.Num() == 0) return FVector(0, 0, 0);
	else if (PickUpPoints.Num() == 1) return PickUpPoints[0];
	else return UCroissantFunctionLibrary::GetClosestPoint(PickUpPoints, ToPoint);
}

void AGroceryActor::TryPlayingPickUpAnimation(AActor* ForActor, FTransform TargetTransform, bool LeftHand /*= true*/)
{
	//Animation stuff. If possible, make it look like character actually grabs an object
	if (ForActor->GetClass()->ImplementsInterface(UCroissantInterface::StaticClass())) {
		UStaticMeshComponent* DummyPickUpObject = NewObject<UStaticMeshComponent>(this);
		DummyPickUpObject->RegisterComponent();
		DummyPickUpObject->SetStaticMesh(ThisItemInfo.MeshRef);
		DummyPickUpObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DummyPickUpObject->SetGenerateOverlapEvents(false);
		DummyPickUpObject->SetWorldTransform(TargetTransform);

		ICroissantInterface::Execute_PlayGrabAnimationForObject(ForActor, DummyPickUpObject, true, true);
	}
	//
}

void AGroceryActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}