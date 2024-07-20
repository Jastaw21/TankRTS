// Fill out your copyright notice in the Description page of Project Settings.

// this
#include "Core/Controller/MyPlayerController.h"

// UE includes
#include "Blueprint/UserWidget.h"

// camera widget, and movement
#include "TankRTS/Public/Core/Camera/CameraWidget.h"
#include "TankRTS/Public/Core/Camera/Components/CameraWidgetMovement.h"

// subcomponents
#include "Core/Controller/Components/UnitCommanderComponent.h"
#include "TankRTS/Public/Core/Controller/Components/CameraControllerComponent.h"

// other TankRTS classes
#include "TankRTS/Public/Core/UI/HUD/TankRTSHud.h"
#include "TankRTS/Public/Core/UI/Widgets/SelectionUIWidget.h"

// stuff that happens early
AMyPlayerController::AMyPlayerController()
{
    // deal with the mouse stuff
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Crosshairs;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    // cache a ptr to the Camera Widget to make life easier
    APawn* ControlledPawnTemp = GetPawn();
    ControlledCameraWidget = Cast<ATankWidget, APawn>(ControlledPawnTemp);

    // set up the subcomponent that just looks after the camera controls
    CameraControllerComponent = CreateDefaultSubobject<UCameraControllerComponent>(TEXT("Camera Controller"));
    // pass it some ptrs to stuff
    CameraControllerComponent->GetComponents(ControlledCameraWidget, this);

    // set up the subcomponent that will handle selecting and ordering of units
    UnitCommanderComponent = CreateDefaultSubobject<UUnitCommanderComponent>(TEXT("Unit Controls"));

    DefaultClickTraceChannel = ECollisionChannel::ECC_GameTraceChannel2;
}
void AMyPlayerController::SetupInputComponent()
{ // set up gameplay key bindings
    Super::SetupInputComponent();
}

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bEnableClickEvents = true; // we want to track mouse clicks for selection etc

    // cache a pointer to the camera widget to make referring to it easier
    APawn* ControlledPawnTemp = GetPawn();
    ControlledCameraWidget = Cast<ATankWidget, APawn>(ControlledPawnTemp);
    if (ControlledCameraWidget) {
        CameraControllerComponent->GetComponents(ControlledCameraWidget, this);
    }

    // offload the inputs to the sub controller
    if (InputComponent && GEngine) {
        CameraControllerComponent->SetControlInputs(InputComponent);
        UnitCommanderComponent->SetControlInputs(InputComponent);
    }

    // build the UI
    if (PlayerUIClass) {
        PlayerUI = CreateWidget<USelectionUIWidget>(this, PlayerUIClass);
        check(PlayerUI);
        PlayerUI->AddToPlayerScreen();
    }
}

void AMyPlayerController::NotifyUnitsAreSelected()
{
    UnitCommanderComponent->FetchHUDSelectedUnits();
}

ATankRTSHud* AMyPlayerController::GetHUDCasted()
{
    AHUD* HudTemp = GetHUD();
    ATankRTSHud* Hud = Cast<ATankRTSHud, AHUD>(HudTemp);
    return Hud;
}
