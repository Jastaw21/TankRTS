// Copyright Jack Wedell 2024

#pragma once

#include "CoreMinimal.h"
#include "Core/Units/Base/RTSInteractableBase.h"
#include "RTSBuilding.generated.h"

/**
 Base Building class
 */
UCLASS()
class TANKRTS_API ARTSBuilding : public ARTSInteractableBase
{
	GENERATED_BODY()


public:
	ARTSBuilding();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick( float DeltaTime ) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;



	// interface functions
	virtual ARTSInteractableBase* SelectRTS_Implementation() override;

	
};
