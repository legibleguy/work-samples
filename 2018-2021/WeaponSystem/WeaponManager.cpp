// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManager.h"

// Sets default values for this component's properties
UWeaponManager::UWeaponManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    
    CurrentWeaponRightIndex = -1;
    CurrentWeaponLeftIndex = -1;
    RightHandSocket = "hand_r";
    LeftHandSocket = "hand_l";

	// ...
}


// Called when the game starts
void UWeaponManager::BeginPlay()
{
	Super::BeginPlay();
    
    Char = Cast<ACharacter>(GetOwner());
    if(Char){
        OwnerMesh = Char->GetMesh();
    }
    else DestroyComponent();

	// ...
	
}


// Called every frame
void UWeaponManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponManager::GetWeaponWithIndex(int32 InIndex, bool LeftHand){
    FName Socket = LeftHand ? LeftHandSocket : RightHandSocket;
    int32 Index = InIndex;
    if(!WeaponsInventory.IsValidIndex(Index) && (Index != -1)) Index = -1;
    AActor* ActorToClear = LeftHand ? CurrentWeaponLeft : CurrentWeaponRight;
    if(ActorToClear) {
        ActorToClear->Destroy();
        ActorToClear = NULL;
    }
	auto targetClass = WeaponsInventory.IsValidIndex(Index) ? WeaponsInventory[Index].WeaponClass : NULL;
	if (!targetClass) Index = -1;
    if(Index != -1){
        //Gathering spawn info for the new weapon
        FTransform NewTr;
        if(OwnerMesh){
            NewTr = OwnerMesh->USceneComponent::GetSocketTransform(Socket);
        }
    
        //Spawning The New Weapon
        auto ClassToSpawn = Cast<UClass>(targetClass);
        auto NewWeapon = GetWorld()->SpawnActorDeferred<AActor>(ClassToSpawn, NewTr);
    
        AWeaponActor* AsWeapon = Cast<AWeaponActor>(NewWeapon);
        
        AsWeapon->UpdateOwner(Char);
        
        AsWeapon->IsInRightHand = !LeftHand;
        
        AsWeapon->FinishSpawning(NewTr);
        
        NewWeapon->AttachToComponent(OwnerMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, Socket);
        
        FTransform RelTr = LeftHand ? AsWeapon->ForLeftHand : AsWeapon->ForRightHand;
        AsWeapon->SetActorRelativeTransform(RelTr);
    
        if(LeftHand) CurrentWeaponLeft = AsWeapon;
        else CurrentWeaponRight = AsWeapon;
    }
	if (LeftHand) {
		if (CurrentWeaponLeftIndex != Index && WeaponChanged.IsBound()) {
			WeaponChanged.Broadcast(CurrentWeaponLeftIndex, Index, true);
			CurrentWeaponLeftIndex = Index;
		};
	}
	else {
		if (CurrentWeaponRightIndex != Index && WeaponChanged.IsBound()){
			WeaponChanged.Broadcast(CurrentWeaponRightIndex, Index, false);
			CurrentWeaponRightIndex = Index;
		}
	}
}

bool UWeaponManager::GetWeaponOfClass(TSubclassOf<class AWeaponActor> WeaponClass, bool LeftHand /*= false*/)
{
	for (int32 i = 0; i < WeaponsInventory.Num(); i++) {
		if (WeaponsInventory[i].WeaponClass == WeaponClass) {
			GetWeaponWithIndex(i, LeftHand);
			return true;
		}
	}
	return false;
}

bool UWeaponManager::GetWeaponByName(FName InName, bool LeftHand /*= false*/)
{
	for (int32 i = 0; i < WeaponsInventory.Num(); i++) {
		if (WeaponsInventory[i].CustomName == InName) {
			GetWeaponWithIndex(i, LeftHand);
			return true;
		}
	}
	return false;
}

void UWeaponManager::GetNextWeapon(bool LeftHand /*= false*/)
{
	int32 maybeNextIndex = (LeftHand ? CurrentWeaponLeftIndex : CurrentWeaponRightIndex) + 1;
	if (maybeNextIndex > WeaponsInventory.Num() - 1 || maybeNextIndex < 0) maybeNextIndex = 0;
	GetWeaponWithIndex(maybeNextIndex, LeftHand);
}

void UWeaponManager::GetPrevWeapon(bool LeftHand /*= false*/)
{
	int32 maybeNextIndex = (LeftHand ? CurrentWeaponLeftIndex : CurrentWeaponRightIndex) - 1;
	if (maybeNextIndex < 0 || maybeNextIndex > WeaponsInventory.Num() - 1) maybeNextIndex = WeaponsInventory.Num() - 1;
	GetWeaponWithIndex(maybeNextIndex, LeftHand);
}
