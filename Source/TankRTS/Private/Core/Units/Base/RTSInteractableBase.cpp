// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Units/Base/RTSInteractableBase.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY(RTSInteractables)

// Sets default values
ARTSInteractableBase::ARTSInteractableBase()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Capsule"));
    SetRootComponent(CapsuleComponent);

    SelectionDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Selected Decal"));
    SelectionDecal->SetVisibility(false);
    SelectionDecal->SetupAttachment(RootComponent);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);

    UIThumbnail = CreateDefaultSubobject<UTexture2D>(TEXT("UI Thumbnail"));
}

// Called when the game starts or when spawned
void ARTSInteractableBase::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ARTSInteractableBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARTSInteractableBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FString* ARTSInteractableBase::GetGameplayName()
{
    return &InteractableDetails.GamePlayName;
}

InteractableType ARTSInteractableBase::GetInteractableType()
{
    return InteractableDetails.Interact_Type;
}

void ARTSInteractableBase::Speak()
{
    UE_LOG(RTSInteractables, Display, TEXT("Interactable, gp name: %s, speaking"), GetGameplayName());
}

ARTSInteractableBase* ARTSInteractableBase::OnClickedRTS()
{
    return nullptr;
}

ARTSInteractableBase* ARTSInteractableBase::OnDroppedRTS()
{
    return nullptr;
}
