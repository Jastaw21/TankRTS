// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Units/Base/RTSInteractableBase.h"



#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY(RTSInteractables)

// Sets default values
ARTSInteractableBase::ARTSInteractableBase()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Collision Capsule"));
    SetRootComponent(DefaultRootComponent);

    SelectionDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Selected Decal"));
    SelectionDecal->SetVisibility(false);
    SelectionDecal->SetupAttachment(RootComponent);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);

    BasicCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Primary Collision Box"));
    BasicCollisionBox->SetupAttachment(RootComponent);

    BasicCollisionBox->SetCollisionResponseToChannel( ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);


    UIThumbnail
        = CreateDefaultSubobject<UTexture2D>(TEXT("UI Thumbnail"));
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

FString& ARTSInteractableBase::GetGameplayName()
{
    return InteractableDetails.GamePlayName;
}

InteractableType ARTSInteractableBase::GetInteractableType()
{
    return InteractableDetails.Interact_Type;
}

void ARTSInteractableBase::Speak()
{
    // UE_LOG(RTSInteractables, Display, TEXT("Interactable, gp name: %s, speaking"), GetGameplayName());
    GEngine->AddOnScreenDebugMessage(192, 1.0f, FColor::Red, FString::Printf(TEXT("Interactable, gp name: %s, speaking"), *GetGameplayName()));
}

ARTSInteractableBase* ARTSInteractableBase::OnClickedRTS()
{
    return SelectRTS_Implementation();
}

ARTSInteractableBase* ARTSInteractableBase::OnDroppedRTS()
{
    DeselectRTS_Implementation();
    return nullptr;
}

ARTSInteractableBase* ARTSInteractableBase::SelectRTS_Implementation()
{
    SelectionDecal->SetVisibility(true);
    return this;
}

void ARTSInteractableBase::DeselectRTS_Implementation()
{
    SelectionDecal->SetVisibility(false);
}
