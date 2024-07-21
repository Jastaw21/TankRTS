// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControllerInteractableInterface.generated.h"


/**
Interface to allow the controller to interact at a base level with the interactables
 */


class ARTSInteractableBase;

UINTERFACE( MinimalAPI )
class UContInter : public UInterface{
	GENERATED_BODY()

	};



class TANKRTS_API IContInter{
	GENERATED_BODY()
public:
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent, Category = "Selection/Deselection" )
	AUnitBase* SelectUnit();
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent, Category = "Selection/Deselection" )
	void DeselectUnit();

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent, Category = "Navigation" )
	void SetDestination( FVector Dest );


	};



