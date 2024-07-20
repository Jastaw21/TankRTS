// Fill out your copyright notice in the Description page of Project Settings.

// this
#include "Core/Controller/MyPlayerController.h"
// UE includes
#include "AIController.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"

// camera widget, and movement
#include "TankRTS/Public/Core/Camera/CameraWidget.h"
#include "TankRTS/Public/Core/Camera/Components/CameraWidgetMovement.h"

// subcomponents
#include "Core/Controller/Components/UnitCommanderComponent.h"
#include "TankRTS/Public/Core/Controller/Components/CameraControllerComponent.h"

// other TankRTS classes
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "TankRTS/Public/Core/AI/UnitAIControllerBase.h"
#include "TankRTS/Public/Core/UI/HUD/HUDControllerInterface.h"
#include "TankRTS/Public/Core/UI/HUD/TankRTSHud.h"
#include "TankRTS/Public/Core/UI/Widgets/SelectionUIWidget.h"
#include "TankRTS/Public/Core/Units/Base/UnitBase.h"

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

    CameraControllerComponent = CreateDefaultSubobject<UCameraControllerComponent>(TEXT("Camera Controller"));
    CameraControllerComponent->GetComponents(ControlledCameraWidget, this);

    UnitCommanderComponent = CreateDefaultSubobject<UUnitCommanderComponent>(TEXT("Unit Controls"));

    DefaultClickTraceChannel = ECollisionChannel::ECC_GameTraceChannel2;
}
void AMyPlayerController::SetupInputComponent()
{ // set up gameplay key bindings
    Super::SetupInputComponent();

    // mouse movements
    InputComponent->BindAxis("YawSpringArm", this, &AMyPlayerController::UpdateMouseXY);
    InputComponent->BindAxis("PitchSpringArm", this, &AMyPlayerController::UpdateMouseXY);

    // toggle the marquee selection
    InputComponent->BindAction("ToggleMarquee", EInputEvent::IE_Pressed, this, &AMyPlayerController::ToggleMarqueeOn);
    InputComponent->BindAction("ToggleMarquee", EInputEvent::IE_Released, this, &AMyPlayerController::ToggleMarqueeOff);
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

void AMyPlayerController::UpdateMouseXY(float Value)
{
    if (IsMouseHeld) {
        GetMousePosition(MouseEnd.X, MouseEnd.Y);
        StartRectangleDrawing();
    }
}
void AMyPlayerController::TrackMousePosition()
{

    if (!IsMouseHeld) {
        IsMouseHeld = true;
        GetMousePosition(MouseStart.X, MouseStart.Y);
        GetMousePosition(MouseEnd.X, MouseEnd.Y);
    }

    else {
        GetMousePosition(MouseEnd.X, MouseEnd.Y);
    }

    StartRectangleDrawing();
}

// communicate with the HUD to start/end selecting
void AMyPlayerController::StartRectangleDrawing()
{
    GetHUDCasted()->AskToDrawRect(MouseStart.X, MouseStart.Y, MouseEnd.X, MouseEnd.Y, this);
}
void AMyPlayerController::EndRectangleDrawing()
{
    IsMouseHeld = false;
    GetHUDCasted()->StopDrawingRect();
}

void AMyPlayerController::NotifyUnitsAreSelected()
{
    return;
}

ATankRTSHud* AMyPlayerController::GetHUDCasted()
{
    AHUD* HudTemp = GetHUD();
    ATankRTSHud* Hud = Cast<ATankRTSHud, AHUD>(HudTemp);
    return Hud;
}
