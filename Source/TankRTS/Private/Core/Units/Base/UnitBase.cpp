// Fill out your copyright notice in the Description page of Project Settings.

#include "TankRTS/Public/Core/Units/Base/UnitBase.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/Units/Components/UnitNavMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "TankRTS/Public/Core/AI/UnitAIControllerBase.h"
#include "TankRTS/Public/Core/Units/Components/BaseUnitMovement.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TankRTS/Public/Core/AI/UnitAIControllerBase.h"

// Sets default values
AUnitBase::AUnitBase()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Capsule"));
    SetRootComponent(CapsuleComponent);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
    Mesh->SetCollisionProfileName("Unitss");
    Mesh->bReceivesDecals = false;

    Mesh->SetupAttachment(GetRootComponent());

    MovementComponent = CreateDefaultSubobject<UUnitNavMovementComponent>(TEXT("New Movement"));
    MovementComponent->SetUpdatedComponent(RootComponent);

    SelectionDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Selected Decal"));
    SelectionDecal->SetupAttachment(RootComponent);
    SelectionDecal->SetVisibility(false);

    UIThumbnail = CreateDefaultSubobject<UTexture2D>(TEXT("UI Thumbnail"));
}

// Called when the game starts or when spawned
void AUnitBase::BeginPlay()
{
    Super::BeginPlay();
}

AUnitBase* AUnitBase::SelectUnit_Implementation()
{
    SelectionDecal->SetVisibility(true);
    return this;
}

void AUnitBase::DeselectUnit_Implementation()
{
    SelectionDecal->SetVisibility(false);
}

void AUnitBase::SetDestination_Implementation(FVector Destination)
{
    MovementDetails.DesiredLocation = Destination;
    MovementDetails.HasDesiredLocation = true;

    AController* ControllerPtr = GetController();

    if (ControllerPtr) {
        AUnitAIControllerBase* AIController = Cast<AUnitAIControllerBase, AController>(ControllerPtr);

        if (AIController) {

            AIController->GetBlackboardComponent()->SetValueAsVector(FName("ClickedLocation"), Destination);
        }
    }
}

UUnitNavMovementComponent* AUnitBase::GetNavMovement()
{
    return MovementComponent;
}

// Called every frame
void AUnitBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AUnitBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

DesiredUnitMovementDetails& AUnitBase::GetMovementDetails()
{
    return MovementDetails;
}
