// Fill out your copyright notice in the Description page of Project Settings.

#include "TankRTS/Public/Core/Units/Base/UnitBase.h"
#include "Components/BoxComponent.h"

#include "Components/BoxComponent.h"
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

    // set up mesh, and use as root
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);
    Mesh->bReceivesDecals = false;

    // main collision box
    BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Capsule"));
    BoxCollisionComponent->SetupAttachment(RootComponent);

    // forward box - mostly as a source point for a line trace
    ForwardCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Forward Collision Capsule"));
    ForwardCollisionComponent->SetupAttachment(RootComponent);

    // aft version of the above
    AftCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Aft Collision Capsule"));
    AftCollisionComponent->SetupAttachment(RootComponent);

    // custom navigation component
    MovementComponent = CreateDefaultSubobject<UUnitNavMovementComponent>(TEXT("New Movement"));
    MovementComponent->SetUpdatedComponent(RootComponent);

    // marks the floor when unit selected
    SelectionDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Selected Decal"));
    SelectionDecal->SetupAttachment(RootComponent);
    SelectionDecal->SetVisibility(false);

    // set the default AI behaviour to be rest
    AController* ControllerPtr = GetController();

    //if (ControllerPtr) {
    //    AUnitAIControllerBase* AIController = Cast<AUnitAIControllerBase, AController>(ControllerPtr);

    //    if (AIController) {
    //        AIController->GetBlackboardComponent()->SetValueAsEnum(FName("UnitStatus"), UnitAIStatus::Rest);
    //    }
    //}
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
            GEngine->AddOnScreenDebugMessage(7, 1.0f, FColor::Cyan, TEXT("Asking Unit to Move"));
            AIController->GetBlackboardComponent()->SetValueAsVector(FName("ClickedLocation"), Destination);
            UnitStatus = UnitAIStatus::MovingTo;
           
        }
    }
}

void AUnitBase::SetStatus_Implementation( UnitAIStatus Status )
    {
   UnitStatus = Status;
    }

UnitAIStatus AUnitBase::GetUnitStatus_Implementation()
   {
   return UnitStatus;
   }

UBoxComponent* AUnitBase::GetForwardCollisionBox()
{
    return ForwardCollisionComponent;
}

UBoxComponent* AUnitBase::GetAftCollisionBox()
{
    return AftCollisionComponent;
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
