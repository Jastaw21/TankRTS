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
class UBoxComponent;

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

   
public:
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* Mesh; // the main unit mesh

    UPROPERTY(EditAnywhere)
    UUnitNavMovementComponent* MovementComponent;

    UPROPERTY(EditAnywhere)
    UDecalComponent* SelectionDecal;

  

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBoxComponent* BoxCollisionComponent;

    UPROPERTY( EditAnywhere, BlueprintReadWrite )
    UBoxComponent* ForwardCollisionComponent;


    UPROPERTY( EditAnywhere, BlueprintReadWrite )
    UBoxComponent* AftCollisionComponent;


    // unit<->controller interface implementations
    virtual AUnitBase* SelectUnit_Implementation() override;
    virtual void DeselectUnit_Implementation() override;
    virtual void SetDestination_Implementation(FVector Destination) override;


    UBoxComponent* GetForwardCollisionBox();
    UBoxComponent* GetAftCollisionBox();
    



    UUnitNavMovementComponent* GetNavMovement();

    FORCEINLINE void SetIsUnitMoving(bool IsUnitMoving_p) { IsUnitMoving = IsUnitMoving_p; }
    FORCEINLINE bool GetIsUnitMoving() { return IsUnitMoving; }

    DesiredUnitMovementDetails& GetMovementDetails();
    FORCEINLINE void CancelMovement() { MovementDetails.HasDesiredLocation = false; }
    FORCEINLINE bool IsMoveable() const { return MovementDetails.HasDesiredLocation; }

private:
    DesiredUnitMovementDetails MovementDetails;
    bool IsUnitMoving = false;
};
