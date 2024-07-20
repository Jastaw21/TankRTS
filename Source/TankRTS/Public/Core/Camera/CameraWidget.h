// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

// ..
// ..
#include "CameraWidget.generated.h"

class UCameraWidgetMovement;
class USceneComponent;
class USpringArmComponent;
class UCameraComponent;


UCLASS()
class TANKRTS_API ATankWidget : public APawn {
    GENERATED_BODY()
public:
    // Sets default values for this pawn's properties
    ATankWidget();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent) override;

    // end engine code
    // xx
    // xx
    // begin custom code

public:
    // just put these here because they appeared in the default
    /** Returns TopDownCameraComponent subobject **/
    FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const
    {
        return CameraComponent;
    }
    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const
    {
        return SpringArmComponent;
    }

    

    // components
    // camera
    UPROPERTY(EditAnywhere, Category = Camera)
    UCameraComponent* CameraComponent;

    // spring arm
    UPROPERTY(EditAnywhere, Category = Camera)
    USpringArmComponent* SpringArmComponent;

    // root component
    UPROPERTY(EditAnywhere, Category = Camera)
    USceneComponent* DefScenComp;

    // camera movement component
    UPROPERTY(EditAnywhere, Category = Camera);
    UCameraWidgetMovement* CameraMovementComp;

    virtual UPawnMovementComponent* GetMovementComponent() const override;
    UCameraWidgetMovement* GetCameraMovement();

    UPROPERTY(EditAnywhere, Category = "Boundaries") // Expressed as a % of the screen dimensions
    float EdgeZoneSize = 0.2f;

private:
    bool bShouldAllowCameraYawAndPitch = false;
    
};
