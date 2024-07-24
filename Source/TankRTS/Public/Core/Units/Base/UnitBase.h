// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/DecalComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankRTS/Public/Core/UI/HUD/HUDControllerInterface.h"
#include "TankRTS/Public/Core/Utility/DesiredUnitMovementDetails.h"
// ..
// ..
#include "UnitBase.generated.h"


class UUnitNavMovementComponent;
class UCapsuleComponent;

UCLASS()
class TANKRTS_API AUnitBase : public APawn, public IHUDController {
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    AUnitBase();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    FORCEINLINE UTexture2D* GetUIImage() { return UIThumbnail; }

public:
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* Mesh; // the main unit mesh

    UPROPERTY(EditAnywhere)
    UUnitNavMovementComponent* MovementComponent;

    UPROPERTY(EditAnywhere)
    UDecalComponent* SelectionDecal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* UIThumbnail;

    UPROPERTY( EditAnywhere, BlueprintReadWrite )
    TObjectPtr<UCapsuleComponent> CapsuleComponent;   

    // unit<->controller interface implementations
    virtual AUnitBase* SelectUnit_Implementation() override;
    virtual void DeselectUnit_Implementation() override;
    virtual void SetDestination_Implementation(FVector Destination) override;

    UUnitNavMovementComponent* GetNavMovement();

    DesiredUnitMovementDetails& GetMovementDetails();
    FORCEINLINE void CancelMovement() { MovementDetails.HasDesiredLocation = false; }
    FORCEINLINE bool IsMoveable() { return MovementDetails.HasDesiredLocation; }

private:
    DesiredUnitMovementDetails MovementDetails;
};
