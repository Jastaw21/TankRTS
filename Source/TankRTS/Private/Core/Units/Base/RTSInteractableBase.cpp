// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Units/Base/RTSInteractableBase.h"
#include "Core/Interactables/InteractableDetails.h"

// Sets default values
ARTSInteractableBase::ARTSInteractableBase()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SelectionDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Selected Decal"));
    SelectionDecal->SetVisibility( false );

    UIThumbnail = CreateDefaultSubobject<UTexture2D>( TEXT( "UI Thumbnail" ) );
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

