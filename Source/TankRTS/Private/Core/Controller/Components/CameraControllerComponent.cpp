// Fill out your copyright notice in the Description page of Project Settings.

#include "TankRTS/Public/Core/Controller/Components/CameraControllerComponent.h"
#include "TankRTS/Public/Core/Camera/CameraWidget.h"
#include "TankRTS/Public/Core/Camera/Components/CameraWidgetMovement.h"
#include "TankRTS/Public/Core/Controller/MyPlayerController.h"



// Sets default values for this component's properties
UCameraControllerComponent::UCameraControllerComponent()
{

    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;    
}

void UCameraControllerComponent::SetControlInputs(UInputComponent* InputComponent)
{
 
    // bind camera controls
    InputComponent->BindAxis("ZoomSpringArm", this, &UCameraControllerComponent::AddZoomInput);
    InputComponent->BindAxis("MapLeft", this, &UCameraControllerComponent::AddLateralMapInput);
    InputComponent->BindAxis("MapUp", this, &UCameraControllerComponent::AddVerticalMapInput);

    InputComponent->BindAction("RightMouse", IE_Pressed, this, &UCameraControllerComponent::ToggleYawAllowed);   
    InputComponent->BindAction("RightMouse", IE_Released, this, &UCameraControllerComponent::ToggleYawAllowed);
    InputComponent->BindAction( "Test", IE_Pressed, this, &UCameraControllerComponent::Speak);
    InputComponent->BindAxis("YawSpringArm", this, &UCameraControllerComponent::HandleLateralMouse);
    InputComponent->BindAxis("PitchSpringArm", this, &UCameraControllerComponent::HandleVerticalMouse);
}

// Called when the game starts
void UCameraControllerComponent::BeginPlay()
{
    Super::BeginPlay();
    if (OwningController) {
        UpdatedComponent = OwningController->GetControlledCameraWidget();
    }

    else {
        AActor* tempOwner = GetOwner();
        OwningController = Cast<AMyPlayerController, AActor>(tempOwner);
    }

    // ...
}

// Called every frame
void UCameraControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UCameraControllerComponent::AddZoomInput(float Value)
{

    if (UpdatedComponent) {

        UpdatedComponent->GetCameraMovement()->JAddZoomInput(Value);
    } else if (OwningController) {
        UpdatedComponent = OwningController->GetControlledCameraWidget();
    }
}

void UCameraControllerComponent::AddLateralMapInput(float Value)
{
    if (UpdatedComponent) {
        UpdatedComponent->GetCameraMovement()->JAddLateralInput(UpdatedComponent->GetActorRightVector() * Value);
    } else if (OwningController) {
        UpdatedComponent = OwningController->GetControlledCameraWidget();
    }
}

void UCameraControllerComponent::AddVerticalMapInput(float Value)
{
    if (UpdatedComponent) {
        UpdatedComponent->GetCameraMovement()->JAddLateralInput(UpdatedComponent->GetActorForwardVector() * Value);
    }
}

void UCameraControllerComponent::HandleLateralMouse(float Value)
{
     if (bCameraAllowPitchYaw) {
         UpdatedComponent->GetCameraMovement()->JAddYawInput(Value);
     }

     else if (GEngine) {
         EdgeScroll(Value);
     }
}

void UCameraControllerComponent::HandleVerticalMouse(float Value)
{
     if (bCameraAllowPitchYaw) {
         UpdatedComponent->GetCameraMovement()->JAddPitchInput(Value);
     }

     else if (GEngine) {
         EdgeScroll(Value);
     }
}

void UCameraControllerComponent::EdgeScroll(float Value)
{ // get mouse location
    float mouseX;
    float mouseY;
    OwningController->GetMousePosition(mouseX, mouseY);

    FVector2D ViewportSize;
    // and then get the viewport size
    if (GEngine != nullptr) {
        ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
    } else {
        ViewportSize = FVector2D(100, 100);
    }

    // calculate the %s
    const float xPercent = mouseX / ViewportSize.X;
    const float yPercent = mouseY / ViewportSize.Y;

    if (IsInEdgeZone(xPercent, yPercent))

    {
        float HorizontalInterp = 0.0f;
        float VerticalInterp = 0.0f;
        CalculateInterpolations(xPercent, yPercent, HorizontalInterp, VerticalInterp);
        UpdatedComponent->GetCameraMovement()
            ->JAddLateralInput(UpdatedComponent
                                   ->GetActorForwardVector()
                    * VerticalInterp
                + UpdatedComponent->GetActorRightVector() * HorizontalInterp);
    }
}

bool UCameraControllerComponent::IsInEdgeZone(const float& xPercent, const float& yPercent)
{
    return (xPercent <= EdgeZoneSize || xPercent >= (1 - EdgeZoneSize) || yPercent <= EdgeZoneSize || yPercent >= (1 - EdgeZoneSize));
}

void UCameraControllerComponent::CalculateInterpolations(const float& xPercent, const float& yPercent, float& InHoriz, float& InVertical)
{
    if (xPercent <= EdgeZoneSize) {
        // range should be from 0-0.x (x being edge zone size)
        const float ScaleAcrossEdgeZone = (EdgeZoneSize - xPercent) / EdgeZoneSize;
        InHoriz = -FMath::Lerp(0.0f, 1.0f, ScaleAcrossEdgeZone);
    }

    else if (xPercent >= (1 - EdgeZoneSize)) {
        // range should be from 0.x-1.0 (x being edge zone size)
        const float ScaleAcrossEdgeZone = (xPercent - (1.0f - EdgeZoneSize)) / EdgeZoneSize;
        InHoriz = FMath::Lerp(0.0f, 1.0f, ScaleAcrossEdgeZone);
    }

    if (yPercent <= EdgeZoneSize) { // range should be from 0-0.y (y being edge zone size)
        const float ScaleAcrossEdgeZone = (EdgeZoneSize - yPercent) / EdgeZoneSize;
        InVertical = FMath::Lerp(0.0f, 1.0f, ScaleAcrossEdgeZone);
    }

    else if (yPercent >= (1 - EdgeZoneSize)) {
        const float ScaleAcrossEdgeZone = (yPercent - (1.0f - EdgeZoneSize)) / EdgeZoneSize;
        InVertical = -FMath::Lerp(0.0f, 1.0f, ScaleAcrossEdgeZone);
    }
}

void UCameraControllerComponent::GetComponents( ATankWidget* UpdatedWidget, AMyPlayerController* OwnContr )
    {
    OwningController = OwnContr;
    UpdatedComponent = UpdatedWidget;
    }

