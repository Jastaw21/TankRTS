// Fill out your copyright notice in the Description page of Project Settings.

#include "TankRTS/Public/Core/Camera/CameraWidget.h"

#include "Camera/CameraComponent.h"
#include "TankRTS/Public/Core/Camera/Components/CameraWidgetMovement.h"
#include "Components/SceneComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/UnrealMath.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ATankWidget::ATankWidget()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve
    // performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Component"));

    // set spring arm and character
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = 900.0f;
    SpringArmComponent->SetRelativeRotation(FRotator(-70.0f, 0.0f, 0.0f));

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Itself"));
    CameraComponent->SetupAttachment(SpringArmComponent);

    CameraMovementComp = CreateDefaultSubobject<UCameraWidgetMovement>(
        TEXT("Custom Camera Movement Component"));
    CameraMovementComp->SetUpdatedSpringArm(GetCameraBoom());
}

// Called when the game starts or when spawned
void ATankWidget::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ATankWidget::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATankWidget::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}


UPawnMovementComponent* ATankWidget::GetMovementComponent() const
{
    return CameraMovementComp;
}

UCameraWidgetMovement* ATankWidget::GetCameraMovement()
{
    return CameraMovementComp;
}

