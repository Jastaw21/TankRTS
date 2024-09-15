// Copyright Jack Wedell 2024

#include "Core/Controller/FormationManager.h"
#include "Core//Units/Base/UnitBase.h"
#include "Core/Controller/Components/UnitCommanderComponent.h"
#include "Core/Controller/MyPlayerController.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

// Sets default values for this component's properties
UFormationManager::UFormationManager()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}

// Called when the game starts
void UFormationManager::BeginPlay()
{
    Super::BeginPlay();

    // ...
}

// Called every frame
void UFormationManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    DrawUnitNumbers();

    // ...
}

UUnitCommanderComponent* UFormationManager::GetUnitCommander()
{
    AActor* ControllerRaw = GetOwner();
    AMyPlayerController* Controller = nullptr;
    UUnitCommanderComponent* UnitCommander = nullptr;

    if (ControllerRaw) {
        Controller = Cast<AMyPlayerController, AActor>(ControllerRaw);
        UnitCommander = Controller->GetUnitCommander();
    }

    return UnitCommander;
}

void UFormationManager::DrawUnitNumbers()
{
    TArray<AUnitBase*>* Units = GetControlledUnits();
    uint16 UnitNumber = 1;

    for (AUnitBase* Unit : *Units) {

        DrawDebugString(GetWorld(), Unit->GetActorLocation() + Unit->GetActorUpVector() * 100.0f, FString::Printf(TEXT("%d"), UnitNumber),(AActor*)0,FColor::Red,0.0f);
        UnitNumber++;
    }
}

TArray<AUnitBase*>* UFormationManager::GetControlledUnits()
{
    return GetUnitCommander()->GetControlledUnitsArray();
}
