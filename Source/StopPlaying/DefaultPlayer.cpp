// Fill out your copyright notice in the Description page of Project Settings.

#include "StopPlaying.h"
#include "DefaultPlayer.h"

// Sets default values
ADefaultPlayer::ADefaultPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADefaultPlayer::BeginPlay()
{
	Super::BeginPlay();

    CheckComponents();
	
}

// Called every frame
void ADefaultPlayer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    UpdateInteractionPrompt();
    UpdateGrabbedComponent();
}

// Called to bind functionality to input
void ADefaultPlayer::SetupPlayerInputComponent(class UInputComponent* Input)
{
	Super::SetupPlayerInputComponent(Input);

    if(!Input)
    {
        UE_LOG(LogTemp, Error, TEXT("%s does not have a UInputComponent"), *GetName());
        return;
    }

    // Hook up axies
    Input->BindAxis("MoveForward", this, &ADefaultPlayer::MoveForward);
    Input->BindAxis("MoveRight", this, &ADefaultPlayer::MoveRight);
    Input->BindAxis("LookPitch", this, &ADefaultPlayer::LookPitch);
    Input->BindAxis("LookYaw", this, &ADefaultPlayer::LookYaw);
    
    // Hook up actions
    Input->BindAction("Jump", IE_Pressed, this, &ADefaultPlayer::Jump);
    Input->BindAction("Push", IE_Pressed, this, &ADefaultPlayer::Push);
    Input->BindAction("Grab", IE_Pressed, this, &ADefaultPlayer::Grab);
    Input->BindAction("Grab", IE_Released, this, &ADefaultPlayer::StopGrab);
}

/**
 * Sets up al needed components
 */
void ADefaultPlayer::CheckComponents()
{
    PhysicsHandle = FindComponentByClass<UPhysicsHandleComponent>();

    if(!PhysicsHandle)
    {
        UE_LOG(LogTemp, Error, TEXT("%s has no UPhysicsHandleComponent"), *GetName());
    }
}

/**
 * Moves the player forward or backward
 */
void ADefaultPlayer::MoveForward(float AxisValue)
{
    AddMovementInput(GetActorForwardVector() * AxisValue);
}

/**
 * Moves the player right or left
 */
void ADefaultPlayer::MoveRight(float AxisValue)
{
    AddMovementInput(GetActorRightVector() * AxisValue);
}

/**
 * Controls the player's camera pitch
 */
void ADefaultPlayer::LookPitch(float AxisValue)
{
    AddControllerPitchInput(AxisValue);
}

/**
 * Controls the player's camera yaw
 */
void ADefaultPlayer::LookYaw(float AxisValue)
{
    AddControllerYawInput(AxisValue);
}

/**
 * Performs a jump
 */
void ADefaultPlayer::Jump()
{
    UCharacterMovementComponent* CharacterMovementComponent = FindComponentByClass<UCharacterMovementComponent>();
    
    if(!CharacterMovementComponent) { return; }

    if(CharacterMovementComponent->IsMovingOnGround())
    {
        CharacterMovementComponent->DoJump(false);
    }
}

/**
 * Gets line trace end
 */
void ADefaultPlayer::GetLineTrace(FVector& Begin, FVector& End)
{
    if(!PhysicsHandle) { return; }
    
    FVector Location;
    FRotator Rotation;
    
    APlayerController* Controller = GetWorld()->GetFirstPlayerController();
    
    if(!Controller) {
        UE_LOG(LogTemp, Error, TEXT("First player controller could not be found"));
        return;
    }

    Controller->GetPlayerViewPoint(Location, Rotation); 

    Begin = Location;
    End = Begin + Rotation.Vector() * LineTraceLength;
}

/**
 * Updates the grabbed component
 */
void ADefaultPlayer::UpdateGrabbedComponent()
{
    FVector Begin;
    FVector End;

    GetLineTrace(Begin, End);

    if(PhysicsHandle->GrabbedComponent)
    {
        PhysicsHandle->SetTargetLocation(End);
    }
}

/**
 * Scan for any physics bodies in reach an return the hit result
 */
const FHitResult ADefaultPlayer::GetFirstPhysicsBodyInReach()
{
    FHitResult LineTraceHit;

    FVector LineTraceBegin;
    FVector LineTraceEnd;

    GetLineTrace(LineTraceBegin, LineTraceEnd);

    // Set up trace query params
    FCollisionQueryParams LineTraceParameters(FName(TEXT("")), false, this);

    GetWorld()->LineTraceSingleByObjectType(
        LineTraceHit,
        LineTraceBegin,
        LineTraceEnd,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        LineTraceParameters
    );

//    DrawDebugLine(
// 		GetWorld(),
//      	LineTraceBegin,
//		LineTraceEnd,
//		FColor(255, 0, 0),
//		false,
//		3.f,
//		3.f,
//		10.f
//	);

    return LineTraceHit;
}

/**
 * Gets first interactive actor in reach
 */
AInteractiveActor* ADefaultPlayer::GetFirstInteractiveActorInReach()
{
    // Line trace and detect any actors withing reach
    FHitResult LineTraceHit = GetFirstPhysicsBodyInReach();
    AActor* ActorHit = LineTraceHit.GetActor();

    if(ActorHit)
    {
        return Cast<AInteractiveActor>(ActorHit);
    }

    return nullptr;
}


/**
 * Updates interaction prompt
 */
void ADefaultPlayer::UpdateInteractionPrompt()
{
    // Suspend interaction prompt while grabbing things
    if(PhysicsHandle->GrabbedComponent) { return; }

    AInteractiveActor* InteractiveActor = GetFirstInteractiveActorInReach();

    if(InteractiveActor)
    {
        OnInteractionPrompt.Broadcast(InteractiveActor->InteractionMessage);
    }
}

/**
 * Try to grab any object within reach
 */
void ADefaultPlayer::Grab()
{
    AInteractiveActor* InteractiveActor = GetFirstInteractiveActorInReach();

    if(InteractiveActor && InteractiveActor->bCanBeGrabbed)
    {
        UPrimitiveComponent* ComponentToGrab = Cast<UPrimitiveComponent>(InteractiveActor->GetRootComponent());

        if(!PhysicsHandle) { return; }

        if(!ComponentToGrab)
        {
            UE_LOG(LogTemp, Error, TEXT("%s has no UPrimitiveComponent"), *InteractiveActor->GetName());
            return;
        }

        PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, InteractiveActor->GetActorLocation(), true);
    }
}

/**
 * Stops grabbing
 */
void ADefaultPlayer::StopGrab()
{
    if(!PhysicsHandle) { return; }

    PhysicsHandle->ReleaseComponent();
}

/**
 * Pushes something in front of the player
 */
void ADefaultPlayer::Push()
{
    if(!PhysicsHandle) { return; }

    // First check for grabbed object
    if(PhysicsHandle->GrabbedComponent)
    {
        UPrimitiveComponent* GrabbedComponent = PhysicsHandle->GrabbedComponent;

        PhysicsHandle->ReleaseComponent();

        FVector Location;
        FRotator Rotation;
        
        APlayerController* Controller = GetWorld()->GetFirstPlayerController();
        
        if(!Controller) {
            UE_LOG(LogTemp, Error, TEXT("First player controller could not be found"));
            return;
        }

        Controller->GetPlayerViewPoint(Location, Rotation); 

        GrabbedComponent->SetPhysicsLinearVelocity(Rotation.Vector() * PushingPower * 100.f);
        
        return;
    }

    // Invoke interaction
    AInteractiveActor* InteractiveActor = GetFirstInteractiveActorInReach();

    if(InteractiveActor && !InteractiveActor->bCanBeGrabbed)
    {
        InteractiveActor->Interact(this);
    }
}
