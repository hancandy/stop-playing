#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "DefaultPlayerMovementComponent.generated.h"

UCLASS()
class STOPPLAYING_API UDefaultPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
    /**
     * Sets the gravity scale and inverts the movement behaviour if necessary
     */
    void SetGravityScale(float NewGravityScale);
	
	
};
