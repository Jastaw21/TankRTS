// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Controller/Components/UnitCommanderComponent.h"
#include "Core/Controller/MyPlayerController.h"
#include "Core/UI/Widgets/SelectionUIWidget.h"
#include "Core/Units/Base/UnitBase.h"
#include "GameFramework/HUD.h"
#include "TankRTS/Public/Core/UI/HUD/TankRTSHud.h"

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
}

// used to see if theres a unit under the cursor
void UUnitCommanderComponent::PollAreaUnderCursor()
{

    // run the unit click test first
    FHitResult UnitHit;
    bool UnitHitSuccess = false;
    UnitHitSuccess = Parent->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1), false, UnitHit);

    if (UnitHitSuccess) {
        AUnitBase* HitUnit = Cast<AUnitBase, AActor>(UnitHit.GetActor());
        SelectUnit(HitUnit);
    }
}

// handlers to be fired
void UUnitCommanderComponent::SelectUnit(AUnitBase* UnitToSelect)
{
    if (UnitToSelect->GetClass()->ImplementsInterface(UHUDController::StaticClass())) {
        SubscribedUnits.Add(IHUDController::Execute_SelectUnit(UnitToSelect));
    }

    if (Parent->GetSelectionUIWidget()) {
        Parent->GetSelectionUIWidget()->IncrementNumUnitsSelected();
    }
}
void UUnitCommanderComponent::GetUnitDestination()
{
    UE_LOG(LogTemp, Display, TEXT("New Double Click Called"));
    if (SubscribedUnits.Num() > 0) {
        FHitResult Hit;
        bool bHitSucc = false;
        bHitSucc = Parent->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

        if (bHitSucc) {
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
}

void UUnitCommanderComponent::DropAllSelectedUnits()
{
    UE_LOG(LogTemp, Display, TEXT("Dropping Units"));
    for (auto& SelectedUnit : SubscribedUnits) {
        IHUDController::Execute_DeselectUnit(SelectedUnit);
    }

    SubscribedUnits.Empty();

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

void UUnitCommanderComponent::StartRectangleDrawing()
{
    bIsSelectionBoxBeingDrawn = true;
    if (Parent)
    {
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