#include "JWMath.h"

using namespace JWMath;

void JWMath::ScaleVectorToSpeed( FRotator& RotatorIn, float MaxSpeed)
{
    // how big is the required rotation in each axis
    float PitchVar = RotatorIn.Pitch;
    float YawVar = RotatorIn.Yaw;

    // and then what percentage of the total magnitude does each axis provide
    float AbsDesiredRotationScale = FMath::Abs(PitchVar) + FMath::Abs(YawVar);
    float PitchScale = FMath::Abs(PitchVar) / AbsDesiredRotationScale;
    float YawScale = FMath::Abs(YawVar) / AbsDesiredRotationScale;

    // return each axis to the scaled place
    float NewPitch = MaxSpeed * PitchScale * FMath::Sign(PitchVar);
    float NewYaw = MaxSpeed * YawScale * FMath::Sign(YawVar);

    RotatorIn.Pitch = NewPitch;
    RotatorIn.Yaw = NewYaw;
}
