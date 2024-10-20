// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Controller/Components/UnitCommanderComponent.h"
#include "Core/Controller/MyPlayerController.h"
#include "Core/Game Mode/RTSGameState.h"
#include "Core/UI/Widgets/SelectionUIWidget.h"
#include "Core/Units/Base/UnitBase.h"
#include "GameFramework/HUD.h"
#include "TankRTS/Public/Core/AI/UnitAIStatus.h"
#include "TankRTS/Public/Core/Interactables/Interfaces/ControllerInteractableInterface.h"
#include "TankRTS/Public/Core/UI/HUD/TankRTSHud.h"
#include "TankRTS/Public/Core/Units/Base/RTSInteractableBase.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

/*
Engine Implementation
*/

// Sets default values for this component's properties
UUnitCommanderComponent::UUnitCommanderComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}
// Called when the game starts
void UUnitCommanderComponent::BeginPlay()
{
    Super::BeginPlay();

    if (Parent == nullptr) {

        AActor* tempOwner = GetOwner();
        Parent = Cast<AMyPlayerController, AActor>(tempOwner);
    }

    // ...
}
// Called every frame
void UUnitCommanderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

/*
Begin RTS Implementation
*/

// called when setting up component
void UUnitCommanderComponent::SetControlInputs(UInputComponent* InputComponent)
{

    InputComponent->BindAction("Click", IE_Pressed, this, &UUnitCommanderComponent::PollAreaUnderCursor);
    InputComponent->BindAction("Click", IE_DoubleClick, this, &UUnitCommanderComponent::GetUnitDestination);
    InputComponent->BindAction("QPressed", IE_Released, this, &UUnitCommanderComponent::DropAllSelectedUnits);

    InputComponent->BindAction("LeftShift", IE_Pressed, this, &UUnitCommanderComponent::StartRectangleDrawing);
    InputComponent->BindAction("LeftShift", IE_Released, this, &UUnitCommanderComponent::EndRectangleDrawing);

    InputComponent->BindAxis("MouseX", this, &UUnitCommanderComponent::UpdateMousePositions);

    InputComponent->BindAction("TogglePatrol", IE_Released, this, &UUnitCommanderComponent::TogglePatrol);
}

void UUnitCommanderComponent::FetchHUDSelectedUnits()
{
    if (GetHUDCasted() != nullptr) {

        TArray<AUnitBase*>* ReturnedArray = GetHUDCasted()->GetSelectedUnits();

        if (ReturnedArray) {
            for (AUnitBase* Unit : *ReturnedArray) {
                SelectUnit(Unit);
            }
        }
    }
}

// used to see if theres a unit under the cursor
void UUnitCommanderComponent::PollAreaUnderCursor()
{

    // run the unit click test first
    FHitResult UnitHit;
    bool UnitHitSuccess = false;
    UnitHitSuccess = Parent->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1), false, UnitHit);

    if (UnitHitSuccess) {

        if (UnitHit.GetActor()->IsA<ARTSInteractableBase>()) {
            GEngine->AddOnScreenDebugMessage(919, 1.0f, FColor { 250, 201, 170, 255 }, TEXT("INTERACTABLE HIT"));

            ARTSInteractableBase* Interactable = Cast<ARTSInteractableBase, AActor>(UnitHit.GetActor());
            Interactable->Speak();
        }
        // AUnitBase* HitUnit = Cast<AUnitBase, AActor>(UnitHit.GetActor());
        // SelectUnit(HitUnit);
    }
}

// handlers to be fired
void UUnitCommanderComponent::SelectUnit(AUnitBase* UnitToSelect)
{

    int PreviousElements = SubscribedUnits.Num();

    if (UnitToSelect->GetClass()->ImplementsInterface(UHUDController::StaticClass())) {

        int32 IndexAdded = SubscribedUnits.AddUnique(IHUDController::Execute_SelectUnit(UnitToSelect));

        UnitRankings.Add(IndexAdded, 2);
    }

    if (Parent->GetSelectionUIWidget()) {

        int NewElements = SubscribedUnits.Num() - PreviousElements;
        Parent->GetSelectionUIWidget()->AddNumSelectedUnits(NewElements);
    }
}

void UUnitCommanderComponent::GetUnitDestination()
{
    if (SubscribedUnits.Num() > 0) {
        FHitResult Hit;
        bool HitSuccessful = false;
        HitSuccessful = Parent->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

        if (HitSuccessful) {
            for (AUnitBase* Unit : SubscribedUnits) {

                if (Unit->GetClass()->ImplementsInterface(UHUDController::StaticClass()))
                    // ask the unit to move there

                    Unit->Execute_SetDestination(Unit, Hit.Location);
            }

            if (IsValid(DestinationRing)) {

                UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DestinationRing, Hit.Location);
            }
        }
    }

    else {
        GEngine->AddOnScreenDebugMessage(6, 3.0f, FColor::Cyan, TEXT("No Units"));
    }
}

void UUnitCommanderComponent::DropAllSelectedUnits()
{
    UE_LOG(LogTemp, Display, TEXT("Dropping Units"));
    for (auto& SelectedUnit : SubscribedUnits) {
        IHUDController::Execute_DeselectUnit(SelectedUnit);
    }

    SubscribedUnits.Empty();
    UnitRankings.Empty();

    if (Parent->GetSelectionUIWidget()) {
        Parent->GetSelectionUIWidget()->ResetNumUnitsSelected();
       
    }
}

void UUnitCommanderComponent::DrawHUDSelectionMarquee()
{
    GetHUDCasted()->AskToDrawRect(MouseStart.X, MouseStart.Y, MouseEnd.X, MouseEnd.Y, Parent);
}

void UUnitCommanderComponent::UpdateMousePositions(float Value)
{
    if (bIsSelectionBoxBeingDrawn) {
        Parent->GetMousePosition(MouseEnd.X, MouseEnd.Y);
        DrawHUDSelectionMarquee();
    }
}

void UUnitCommanderComponent::TogglePatrol()
{
    for (AUnitBase* Unit : SubscribedUnits) {

        GEngine->AddOnScreenDebugMessage(199, 1.0f, FColor::Black, TEXT("Patrol Toggle Hit"));
        UnitAIStatus Status;

        switch (Unit->Execute_GetUnitStatus(Unit)) {
        case UnitAIStatus::Patrolling:
            Status = UnitAIStatus::Rest;
            break;

        case UnitAIStatus::Rest:
            GEngine->AddOnScreenDebugMessage(99, 1.0f, FColor::Magenta, TEXT("Switching to Patrol"));
            Status = UnitAIStatus::Patrolling;
            break;

        case UnitAIStatus::MovingTo:
            Status = UnitAIStatus::Patrolling;
        }

        Unit->Execute_SetStatus(Unit, Status);
    }
}

void UUnitCommanderComponent::StartRectangleDrawing()
{
    bIsSelectionBoxBeingDrawn = true;
    if (Parent) {
        Parent->GetMousePosition(MouseStart.X, MouseStart.Y);
        Parent->GetMousePosition(MouseEnd.X, MouseEnd.Y);
    }
}

void UUnitCommanderComponent::EndRectangleDrawing()
{
    bIsSelectionBoxBeingDrawn = false;
    Parent->GetMousePosition(MouseEnd.X, MouseEnd.Y);
    GetHUDCasted()->StopDrawingRect();
}

// internal helper
ATankRTSHud* UUnitCommanderComponent::GetHUDCasted()
{
    AHUD* HudTemp = Parent->GetHUD();
    if (HudTemp) {
        ATankRTSHud* Hud = Cast<ATankRTSHud, AHUD>(HudTemp);
        return Hud;
    }

    else {
        return nullptr;
    }
}