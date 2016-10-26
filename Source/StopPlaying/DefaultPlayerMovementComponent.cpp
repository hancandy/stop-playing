#include "StopPlaying.h"
#include "DefaultPlayerMovementComponent.h"

void UDefaultPlayerMovementComponent::SetGravityScale(float NewGravityScale)
{
    if(!CharacterOwner) { return; }

    bool bShouldInvert = (NewGravityScale < 0.f && GravityScale > 0.f) || (NewGravityScale > 0.f && GravityScale < 0.f);
    FRotator NewRotator = CharacterOwner->GetActorRotation();

    if(bShouldInvert)
    {
        NewRotator.Roll = 180.f;
    }

    UE_LOG(LogTemp, Warning, TEXT("New rotator: %s"), *NewRotator.ToString());

    CharacterOwner->SetActorRotation(NewRotator);

    GravityScale = NewGravityScale;
}
