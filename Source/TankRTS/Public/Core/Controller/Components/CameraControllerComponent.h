// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
// ..
// ..
#include "CameraControllerComponent.generated.h"

class UInputComponent;
class AMyPlayerController;
class ATankWidget;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TANKRTS_API UCameraControllerComponent : public UActorComponent {
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UCameraControllerComponent();

    void SetControlInputs(UInputComponent* InputComponent);

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    // input handlers
    void AddZoomInput(float Value);
    void AddLateralMapInput(float Value);
    void AddVerticalMapInput(float Value);
    void AddSnapToLocation();
    void FORCEINLINE ToggleYawAllowed() { bCameraAllowPitchYaw = !bCameraAllowPitchYaw; }

    // handle existing inputs
    void HandleLateralMouse(float Value);
    void HandleVerticalMouse(float Value);
    void EdgeScroll(float Value);

    // helper functions
    bool IsInEdgeZone(const float& xPercent, const float& yPercent);
    void CalculateInterpolations(const float& xPercent, const float& YPercent, float& InHoriz, float& InVertical);

    // data to track control of the camers
    bool bCameraAllowPitchYaw = false;
    FVector SnapToLocation { 0, 0, 0 };
    bool bIsSnappingTo;

    AMyPlayerController* OwningController;
    ATankWidget* UpdatedComponent;
    FORCEINLINE void Speak() { UE_LOG(LogTemp, Display, TEXT("Camera Cont Cmp Speaking")); }

public:
    void GetComponents(ATankWidget* UpdatedWidget, AMyPlayerController* OwnContr);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Movement") // expressed as a % of viewport size
    float EdgeZoneSize = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Movement")
    float SnapToAcceptanceRadius = 3.0f;
};
