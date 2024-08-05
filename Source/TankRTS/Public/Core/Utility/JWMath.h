#pragma once

#include "CoreMinimal.h"



// To be used for utility math

namespace JWMath {

/**
	 * Scales a rotator in place - to a given max magnitude
	 * @param	RotatorIn		The rotator to be scaled in place
	 * @param	MaxSpeed			The max mgnitude to which to scale	 
	 */

void ScaleVectorToSpeed(FRotator& RotatorIn, float MaxSpeed);

}
