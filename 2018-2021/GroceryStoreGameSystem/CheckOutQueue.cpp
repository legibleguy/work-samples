// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckOutQueue.h"
#include "Croissant/Public/CroissantFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACheckOutQueue::ACheckOutQueue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	QueueSpline = CreateDefaultSubobject<USplineComponent>(TEXT("QueueSpline"));
	CheckoutSpline = CreateDefaultSubobject<USplineComponent>(TEXT("CheckoutSpline"));
	EntryBox = CreateDefaultSubobject<UBoxComponent>(TEXT("EntryBox"));
	EntryBox->SetBoxExtent(FVector(67.5, 40, 45));
	EntryBox->SetHiddenInGame(false);
	USceneComponent* RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootScene);
	DefaultCharacterSize = 150.f;
	bWaitForCharactersToGoToPosition = false;
	bWaitForCheckOut = false;
}


void ACheckOutQueue::UpdateEntryBoxPosition()
{
	float CurrLen = ReverseQueuePosition ? UsedLenght : QueueSpline->GetSplineLength() - UsedLenght;
	FTransform NewTr = QueueSpline->GetTransformAtDistanceAlongSpline(CurrLen, ESplineCoordinateSpace::World);
	NewTr.AddToTranslation(FVector(0, 0, EntryBox->GetScaledBoxExtent().Z));
	SecondToLastCharTransform = EntryBox->GetComponentTransform();
	EntryBox->SetWorldTransform(NewTr);
	EntryBox->UpdateOverlaps();

	if (LineWasUpdated.IsBound()) {
		LineWasUpdated.Broadcast(NewTr.GetLocation());
	}
}

void ACheckOutQueue::AddPawnToQueue(ACharacter* PawnToAdd)
{
	if (PawnToAdd && !CharactersInTheQueue.Contains(PawnToAdd)) {
		if (PawnToAdd->GetController()->GetClass()->ImplementsInterface(URollingPapersStuff::StaticClass())) {
			const float TargetLen = ReverseQueuePosition ? UsedLenght : QueueSpline->GetSplineLength() - UsedLenght;
			FTransform TargetTr = QueueSpline->GetTransformAtDistanceAlongSpline(TargetLen, ESplineCoordinateSpace::World);
			IRollingPapersStuff::Execute_PawnMoveToLocation(PawnToAdd->GetController(), TargetTr.GetLocation(), TargetTr.GetRotation().Rotator());
			UsedLenght = FMath::Clamp(UsedLenght + DefaultCharacterSize, 0.f, QueueSpline->GetSplineLength());
			UpdateEntryBoxPosition();
			CharactersInTheQueue.Add(PawnToAdd);
			PawnToAdd->Tags.Add("IsInQueue");
			NumCharactersToTrace = CharactersInTheQueue.Num();
		}
	}
}

void ACheckOutQueue::EntryOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!LastOverlappedPC) {
		APawn* AsPawn = Cast<APawn>(OtherActor);
		if (AsPawn) {
			APlayerController* AsPC = Cast<APlayerController>(AsPawn->GetController());
			if (AsPC) {
				LastOverlappedPC = AsPC;
				if (AsPC->GetClass()->ImplementsInterface(URollingPapersStuff::StaticClass())) {
					IRollingPapersStuff::Execute_SendOnScreenMsg(AsPC, FText::FromString("Press E to Enter This Queue"), false, -1.f);
				}
			}
		}
	}
	if (OtherActor->GetClass()->ImplementsInterface(URollingPapersStuff::StaticClass())) {
		IRollingPapersStuff::Execute_BeginOverlapQueue(OtherActor, this);
	}
}

void ACheckOutQueue::EntryEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (LastOverlappedPC) {
		APawn* AsPawn = Cast<APawn>(OtherActor);
		if (AsPawn) {
			if (Cast<APlayerController>(AsPawn->GetController())) {
				IRollingPapersStuff::Execute_RemoveOnScreenMsg(AsPawn->GetController(), FText::FromString("Press E to Enter This Queue"));
				LastOverlappedPC = NULL;
			}
		}
	}
	if (OtherActor->GetClass()->ImplementsInterface(URollingPapersStuff::StaticClass())) {
		IRollingPapersStuff::Execute_EndOverlapQueue(OtherActor, this);
	}
}

bool ACheckOutQueue::PickUpItem_Implementation(APawn* NewOwner)
{
	ACharacter* AsChar = Cast<ACharacter>(NewOwner);
	if (AsChar) {
		AddPawnToQueue(AsChar);
		return true;
	}
	else return false;
}

void ACheckOutQueue::CheckOutFinished_Implementation()
{
	CharactersInTheQueue.RemoveAt(0);
	UsedLenght = FMath::Clamp(UsedLenght - DefaultCharacterSize, 0.f, QueueSpline->GetSplineLength());
	NumCharactersToTrace--;
	bWaitForCheckOut = false;
}

// Called when the game starts or when spawned
void ACheckOutQueue::BeginPlay()
{
	Super::BeginPlay();
	
	EntryBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckOutQueue::EntryOverlap);
	EntryBox->OnComponentEndOverlap.AddDynamic(this, &ACheckOutQueue::EntryEndOverlap);

	UpdateEntryBoxPosition();
}

bool ACheckOutQueue::TraceCharactersInQueue() {
	bool ChangesInTheLine = false;
	FVector TraceLoc;
	FRotator TraceRot;
	float TargetSplineLen = NumCharactersToTrace * DefaultCharacterSize;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceTypes;
	TraceTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);
	int32 LoopMax = NumCharactersToTrace;
	
	FHitResult Trace;
	TArray<ACharacter*> ToRemove;

	for (int32 i = 0; i < LoopMax; i++) {
		TraceLoc = SecondToLastCharTransform.GetLocation();
		TraceRot = SecondToLastCharTransform.GetRotation().Rotator();

		float CurrentSplineLen = TargetSplineLen - (DefaultCharacterSize * (i + 1));
		FTransform CurrentTraceTr = QueueSpline->GetTransformAtDistanceAlongSpline(CurrentSplineLen, ESplineCoordinateSpace::World);
		TraceLoc = CurrentTraceTr.GetLocation() + FVector(0,0, EntryBox->GetScaledBoxExtent().Z);
		TraceRot = CurrentTraceTr.GetRotation().Rotator();

		TArray<AActor*> Ignore(CharactersInTheQueue);
		if (Ignore.IsValidIndex(LoopMax - i - 1))Ignore.RemoveAt(LoopMax - i - 1);
		
		if (!UKismetSystemLibrary::BoxTraceSingleForObjects(this, TraceLoc, TraceLoc, EntryBox->GetScaledBoxExtent(), TraceRot, TraceTypes, false, Ignore, EDrawDebugTrace::ForOneFrame, Trace, true)) {
			ToRemove.Add(CharactersInTheQueue[LoopMax - 1 - i]);
			NumCharactersToTrace--;
			CharactersInTheQueue[LoopMax - 1 - i]->Tags.Remove("IsInQueue");
			UsedLenght = FMath::Clamp(UsedLenght - DefaultCharacterSize, 0.f, QueueSpline->GetSplineLength());
			if(!ChangesInTheLine)ChangesInTheLine = true;
		}
		
	}
	if (ChangesInTheLine) {
		for (int32 g = 0; g < ToRemove.Num(); g++) CharactersInTheQueue.Remove(ToRemove[g]);
		AllCharactersGoToPositions();
	}
	UpdateEntryBoxPosition();

	return ChangesInTheLine;
}

void ACheckOutQueue::AllCharactersGoToPositions()
{
	ACharacter* CurrChar;
	FTransform TargetTr;
	for (int32 i = 0; i < CharactersInTheQueue.Num(); i++) {
		CurrChar = CharactersInTheQueue[i];
		TargetTr = QueueSpline->GetTransformAtDistanceAlongSpline(DefaultCharacterSize * i, ESplineCoordinateSpace::World);

		bool IsInBounds = UKismetMathLibrary::IsPointInBoxWithTransform(CurrChar->GetActorLocation(), TargetTr, EntryBox->GetScaledBoxExtent());

		if (!IsInBounds) {
			if (CurrChar->GetController()->GetClass()->ImplementsInterface(URollingPapersStuff::StaticClass())) {
				IRollingPapersStuff::Execute_PawnMoveToLocation(CurrChar->GetController(), TargetTr.GetLocation(), TargetTr.GetRotation().Rotator());
			}
		}
	}
}

bool ACheckOutQueue::AreAllCharactersInPosition()
{
	ACharacter* CurrChar;
	FTransform TargetTr;
	for (int32 i = 0; i < CharactersInTheQueue.Num(); i++) {
		CurrChar = CharactersInTheQueue[i];
		TargetTr = QueueSpline->GetTransformAtDistanceAlongSpline(DefaultCharacterSize * i, ESplineCoordinateSpace::World);
		TargetTr.AddToTranslation(FVector(0, 0, EntryBox->GetScaledBoxExtent().Z));

		bool IsInBounds = UKismetMathLibrary::IsPointInBoxWithTransform(CurrChar->GetActorLocation(), TargetTr, EntryBox->GetScaledBoxExtent());
		UKismetSystemLibrary::DrawDebugBox(this, TargetTr.GetLocation(), EntryBox->GetScaledBoxExtent(), FLinearColor::Yellow, TargetTr.GetRotation().Rotator(), 0.1, 1);

		UKismetSystemLibrary::PrintString(this, FString::FromInt(i) + ": " + FString::FromInt(IsInBounds), true, true, FLinearColor::Green, 0);

		if (!IsInBounds) return false;
	}
	return true;
}

FVector ACheckOutQueue::GetCurrentEnterLocation() const
{
	return EntryBox->GetComponentLocation();
}

// Called every frame
void ACheckOutQueue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CharactersInTheQueue.Num() > 0 && !bWaitForCharactersToGoToPosition) {
		if (TraceCharactersInQueue()) bWaitForCharactersToGoToPosition = true;
		else {
			if (!bWaitForCheckOut && AreAllCharactersInPosition()) {
				bWaitForCheckOut = true;
				if (CharactersInTheQueue[0] && CharactersInTheQueue[0]->GetClass()->ImplementsInterface(URollingPapersStuff::StaticClass())) {
					IRollingPapersStuff::Execute_Client_InitiateCheckOut(CharactersInTheQueue[0], CashierRef);
				}
				//Initiate Check Out...
			}
		}
	}
	else if (bWaitForCharactersToGoToPosition) {
		UKismetSystemLibrary::PrintString(this, "Waiting", true, true, FLinearColor::Green, 0);
		if (AreAllCharactersInPosition()) bWaitForCharactersToGoToPosition = false;
	}

	UKismetSystemLibrary::PrintString(this, FString::FromInt(CharactersInTheQueue.Num()), true, true, FLinearColor::Red, 0);
}

