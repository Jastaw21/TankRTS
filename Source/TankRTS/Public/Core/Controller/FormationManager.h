// Copyright Jack Wedell 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FormationManager.generated.h"

class UUnitCommanderComponent;
class AUnitBase;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKRTS_API UFormationManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFormationManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void DrawUnitNumbers();


private:


	UUnitCommanderComponent* GetUnitCommander();
	
	TArray<AUnitBase*>* GetControlledUnits();

		
};
