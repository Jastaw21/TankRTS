#include "TankRTS/Public/Core/Utility/DesiredMovementDetails.h"

// Sets default values
ADesiredMovementDetails::ADesiredMovementDetails()
{
	// Set this empty to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADesiredMovementDetails::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADesiredMovementDetails::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADesiredMovementDetails::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
