// Copyright Jack Wedell 2024

#include "RTSBuilding.h"
#include "Components/CapsuleComponent.h"

ARTSBuilding::ARTSBuilding()
{

    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    FInteractableDetails Details;
    Details.Interact_Type = InteractableType::Building;
    Details.GamePlayName =TEXT("Base Building");
    InteractableDetails = Details;

   
}

void ARTSBuilding::BeginPlay()
{
    Super::BeginPlay();
}

void ARTSBuilding::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ARTSBuilding::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

ARTSInteractableBase* ARTSBuilding::SelectRTS_Implementation()
{
    SelectionDecal->SetVisibility(true);
    return this;
}
