#include "StopPlaying.h"
#include "InteractiveActor.h"

// Sets default values
AInteractiveActor::AInteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();

    UMeshComponent* MeshComponent = FindComponentByClass<UMeshComponent>();

    if(!MeshComponent) {
        UE_LOG(LogTemp, Error, TEXT("%s doesn't have a UMeshComponent!"), *GetName());
        return;
    }

    bInitialGravity = MeshComponent->IsGravityEnabled();
}

void AInteractiveActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AInteractiveActor::Interact(APawn* InteractingPawn)
{
    OnInteraction.Broadcast(InteractingPawn);
}

void AInteractiveActor::BeginReset()
{
    OnBeginReset.Broadcast();

    bResetting = true;
}

void AInteractiveActor::Reset(bool bShouldRepeat)
{
    Super::Reset();
    
    FVector Zero;

    UMeshComponent* MeshComponent = FindComponentByClass<UMeshComponent>();

    if(!MeshComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("%s does not have a UMeshComponent"), *GetName());
        return;
    }
   
    MeshComponent->SetPhysicsLinearVelocity(Zero);
    MeshComponent->SetPhysicsAngularVelocity(Zero);
    MeshComponent->SetEnableGravity(bInitialGravity);

    UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(GetRootComponent());
    
    if(!PrimitiveComponent) { return; }
    
    PrimitiveComponent->SetMassScale(NAME_None, 1.f);
    
    bResetting = false;
}

