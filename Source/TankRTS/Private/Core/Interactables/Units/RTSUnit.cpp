// Copyright Jack Wedell 2024

#include "Core/Interactables/Units/RTSUnit.h"

// Sets default values
ARTSUnit::ARTSUnit()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    InteractableDetails.Interact_Type = InteractableType::Unit;
    InteractableDetails.GamePlayName = TEXT("Base Unit");
}

// Called when the game starts or when spawned
void ARTSUnit::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ARTSUnit::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARTSUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

ARTSInteractableBase* ARTSUnit::SelectRTS_Implementation()
{
    Super::SelectRTS_Implementation();

    return this;
}
