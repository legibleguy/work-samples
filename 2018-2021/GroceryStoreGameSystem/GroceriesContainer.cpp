// Fill out your copyright notice in the Description page of Project Settings.


#include "GroceriesContainer.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RollingPapersStuff.h"

// Sets default values
AGroceriesContainer::AGroceriesContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Extent = CreateDefaultSubobject<UBoxComponent>(TEXT("CartExtent"));
	BottomBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BottomBox"));
	EnterBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CartEnter"));
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CartMesh"));
	Inventory = CreateDefaultSubobject<UGroceriesInventory>(TEXT("CartGroceriesInventory"));
	Handle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("CartHandle"));

	InteractionText = FText::FromString("Press E to Use the Cart");

	MainMesh->SetSimulatePhysics(true);

	MainMesh->SetCollisionResponseToAllChannels(ECR_Block);

	SetRootComponent(MainMesh);
	Extent->SetupAttachment(MainMesh);
	BottomBox->SetupAttachment(MainMesh);
	EnterBox->SetupAttachment(MainMesh);

	AvailabilityMultiply = 1.f;
	GrabSocket = "hand_r";
}

bool bActorHoldsContainer(AActor* OtherActor) {
	return OtherActor->Tags.Contains("Shopping");
}

bool AGroceriesContainer::PickUpItem_Implementation(APawn* NewOwner)
{
	ACharacter* AsChar = Cast<ACharacter>(NewOwner);
	if (AsChar) {
		GrabCart(AsChar, UsePhysicsOnGrab);
		return bActorHoldsContainer(AsChar);
	}
	else return false;
}



void AGroceriesContainer::DropItem_Implementation()
{
	ReleaseCart();
}

void AGroceriesContainer::SetOffset(const FVector NewOffset)
{
	Offset = NewOffset;
}

void AGroceriesContainer::SetRotationOffset(const FRotator NewOffset)
{
	RotationOffset = NewOffset;
}

void AGroceriesContainer::GrabCart(ACharacter* NewOwner, bool bUsePhysics /*= true*/)
{
	//Can't grab a cart if it already belongs to someone
	if (CurrOwner || bActorHoldsContainer(NewOwner)) return;

	if(Handle->GetGrabbedComponent())Handle->ReleaseComponent();

	MainMesh->SetSimulatePhysics(false);
	AttachToActor(NewOwner, FAttachmentTransformRules::KeepWorldTransform);
	if (bUsePhysics) {
		MainMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		MainMesh->SetWorldTransform(NewOwner->GetMesh()->GetSocketTransform(GrabSocket), false, NULL, ETeleportType::ResetPhysics);
		MainMesh->SetSimulatePhysics(true);
		if(bFreeRotationDuringPhysics)Handle->GrabComponentAtLocation(MainMesh, NAME_None, MainMesh->GetComponentLocation());
		else Handle->GrabComponentAtLocationWithRotation(MainMesh, NAME_None, MainMesh->GetComponentLocation(), MainMesh->GetComponentRotation());
	}

	else {
		MainMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MainMesh->AttachToComponent(NewOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, GrabSocket);
	}

	auto MaybeCharInv = NewOwner->GetComponentByClass(UGroceriesInventory::StaticClass());
	if (MaybeCharInv) {
		CurrOwnerInv = Cast<UGroceriesInventory>(MaybeCharInv);
		CurrOwnerInv->SubInventories.Add(Inventory);
	}

	SetOwner(NewOwner);
	NewOwner->Tags.Add("Shopping");

	//EnterBox->SetGenerateOverlapEvents(false);
	RemoveMessage(NewOwner);
	CurrOwner = NewOwner;
	CurrAttachSocket = GrabSocket;
	
	if (CurrOwner->GetClass()->ImplementsInterface(URollingPapersStuff::StaticClass())) {
		IRollingPapersStuff::Execute_OnGroceryContainerPickedUp(CurrOwner, this);

		auto OwnerAnim = CurrOwner->GetMesh()->GetAnimInstance();
		if (OwnerAnim && OwnerAnim->GetClass()->ImplementsInterface(URollingPapersStuff::StaticClass())) {
			IRollingPapersStuff::Execute_UpdateShoppingContainerType(OwnerAnim, ContainerType);
		}
	}

}

void AGroceriesContainer::ReleaseCart()
{
	if (Handle->GetGrabbedComponent())Handle->ReleaseComponent();
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	MainMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MainMesh->SetSimulatePhysics(true);

	MainMesh->SetCollisionResponseToAllChannels(ECR_Block);

	CurrOwnerInv->SubInventories.Remove(Inventory);

	if (CurrOwner->GetClass()->ImplementsInterface(URollingPapersStuff::StaticClass())) {
		IRollingPapersStuff::Execute_OnGroceryContainerDropped(CurrOwner);

		auto OwnerAnim = CurrOwner->GetMesh()->GetAnimInstance();
		if (OwnerAnim && OwnerAnim->GetClass()->ImplementsInterface(URollingPapersStuff::StaticClass())) {
			IRollingPapersStuff::Execute_UpdateShoppingContainerType(OwnerAnim, EShoppingContainerType::SCT_NONE);
		}
	}

	CurrOwner->Tags.Remove("Shopping");
	CurrOwnerInv = NULL;
	CurrOwner = NULL;
	CurrAttachSocket = NAME_None;
	SetOwner(NULL);
	
	

	//EnterBox->SetGenerateOverlapEvents(true);
}

void AGroceriesContainer::UpdateGroceryContents(TArray<FGroceryItemInfo> CurrentItems)
{
	Groceries.Reset();

	TArray<AActor*> TraceIgnore;
	FHitResult LastHitRes;

	for (int32 g = 0; g < Inventory->Items.Num(); g++) {
		UStaticMesh* CurrMesh = Inventory->Items[g].MeshRef;
		//UKismetSystemLibrary::PrintString(this, CurrMesh->GetFName().ToString());
		if (CurrMesh) {
			UInstancedStaticMeshComponent* NewISM = NewObject<UInstancedStaticMeshComponent>(this);
			NewISM->RegisterComponent();
			NewISM->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			NewISM->SetStaticMesh(CurrMesh);
			NewISM->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			NewISM->SetCollisionResponseToAllChannels(ECR_Ignore);
			NewISM->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
			NewISM->AttachToComponent(MainMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			NewISM->ResetRelativeTransform();
			for (int32 i = 0; i < Inventory->ItemsAmounts[g]; i++) {

				FVector SpawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(Extent->GetComponentLocation(), Extent->GetScaledBoxExtent());
				FVector BottomPoint = UKismetMathLibrary::RandomPointInBoundingBox(BottomBox->GetComponentLocation(), BottomBox->GetScaledBoxExtent());
				if (UKismetSystemLibrary::LineTraceSingle(this, SpawnPoint, BottomPoint, TraceTypeQuery1, true, TraceIgnore, EDrawDebugTrace::None, LastHitRes, false) && LastHitRes.GetActor() == this) {
					SpawnPoint = LastHitRes.ImpactPoint;
				}
				else SpawnPoint = LastHitRes.TraceEnd;
				NewISM->AddInstanceWorldSpace(FTransform(SpawnPoint));
			}
			Groceries.Add(NewISM);
		}
	}
}

void AGroceriesContainer::ChangeAttachSocket(const FName NewAttachSocket)
{
	if (Handle->GetGrabbedComponent() && CurrOwner) CurrAttachSocket = NewAttachSocket;
}

void AGroceriesContainer::RemoveMessage(AActor* FromActor)
{
	APawn* MaybePawn = Cast<APawn>(FromActor);
	if (MaybePawn) {
		APlayerController* PC = Cast<APlayerController>(MaybePawn->GetController());
		if (PC) {
			IRollingPapersStuff* IF_Ref = new IRollingPapersStuff;
			if (PC->GetClass()->ImplementsInterface(URollingPapersStuff::StaticClass())) {
				IRollingPapersStuff::Execute_RemoveOnScreenMsg(MaybePawn->GetController(), InteractionText);
			}
		}
	}
}

void AGroceriesContainer::EntranceOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!CurrOwner) {
		//First, we make sure that the overlapping actor does not hold any other containers
		if(bActorHoldsContainer(OtherActor)) return;

		APawn* MaybePawn = Cast<APawn>(OtherActor);
		if (MaybePawn) {
			APlayerController* PC = Cast<APlayerController>(MaybePawn->GetController());
			if (PC) {
				LastPC = PC;
				EnableInput(PC);
				if (MaybePawn->GetController()->GetClass()->ImplementsInterface(URollingPapersStuff::StaticClass())) {
					IRollingPapersStuff::Execute_SendOnScreenMsg(MaybePawn->GetController(), InteractionText, false, -1.f);
				}
			}
		}
	}
}

void AGroceriesContainer::EntranceEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	RemoveMessage(OtherActor);
	APawn* MaybePawn = Cast<APawn>(OtherActor);
	if (MaybePawn) {
		APlayerController* PC = Cast<APlayerController>(MaybePawn->GetController());
		if (PC) {
			DisableInput(PC);
		}
	}
}

// Called when the game starts or when spawned
void AGroceriesContainer::BeginPlay()
{
	Super::BeginPlay();

	EnterBox->OnComponentBeginOverlap.AddDynamic(this, &AGroceriesContainer::EntranceOverlap);
	EnterBox->OnComponentEndOverlap.AddDynamic(this, &AGroceriesContainer::EntranceEndOverlap);
	if (Inventory && Extent) {
		Inventory->MaxInventoryExt = Extent->GetScaledBoxExtent() * 2 * AvailabilityMultiply;
		Inventory->OnInventoryUpdated.AddDynamic(this, &AGroceriesContainer::UpdateGroceryContents);
	}
	
}

// Called every frame
void AGroceriesContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Handle->GetGrabbedComponent() && CurrOwner) Handle->SetTargetLocationAndRotation(CurrOwner->GetMesh()->GetSocketLocation(CurrAttachSocket) + Offset, CurrOwner->GetMesh()->GetSocketRotation(CurrAttachSocket) + RotationOffset);
	else if (CurrOwner) {
		FVector CurrOffset = FMath::VInterpTo(MainMesh->GetRelativeLocation(), Offset, DeltaTime, 8.0);
		FRotator CurrRotOffset = FMath::RInterpTo(MainMesh->GetRelativeRotation(), RotationOffset, DeltaTime, 8.0);

		MainMesh->SetRelativeRotation(CurrRotOffset);
		MainMesh->SetRelativeLocation(CurrOffset);
	}

}

