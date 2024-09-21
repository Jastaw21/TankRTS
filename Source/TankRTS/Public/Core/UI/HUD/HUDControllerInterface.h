// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/AI/UnitAIStatus.h"


#include "HUDControllerInterface.generated.h"
/**
 * 
 */

class AUnitBase;

UINTERFACE(MinimalAPI)
class UHUDController : public UInterface{
	GENERATED_BODY()
	
	};



class TANKRTS_API IHUDController
{
	GENERATED_BODY()
public:
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent, Category = "Selection/Deselection" )
	AUnitBase* SelectUnit();
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent, Category = "Selection/Deselection" )
	void DeselectUnit();

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent, Category = "Navigation" )
	void SetDestination( FVector Dest );

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent, Category = "Navigation" )
	void SetStatus( UnitAIStatus UnitStatus );

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent, Category = "Navigation" )
	UnitAIStatus GetUnitStatus();

};
