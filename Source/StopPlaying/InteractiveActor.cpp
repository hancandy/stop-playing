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

    InitialTransform = GetTransform();

    UMeshComponent* MeshComponent = FindComponentByClass<UMeshComponent>();

    if(!MeshComponent) {
        UE_LOG(LogTemp, Error, TEXT("%s doesn't have a UMeshComponent!"), *GetName());
        return;
    }

    bInitialGravity = MeshComponent->IsGravityEnabled();
    
    UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(GetRootComponent());
    
    if(!PrimitiveComponent) {
        UE_LOG(LogTemp, Error, TEXT("%s has no UPrimitiveComponent!"), *GetName());
        return;
    }

    bInitialCollision = PrimitiveComponent->GetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody) == ECollisionResponse::ECR_Block;
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

void AInteractiveActor::Reset()
{
    SetActorTransform(InitialTransform);

    FVector Zero;

    UMeshComponent* MeshComponent = FindComponentByClass<UMeshComponent>();

    if(!MeshComponent) { return; }
   
    MeshComponent->SetPhysicsLinearVelocity(Zero);
    MeshComponent->SetPhysicsAngularVelocity(Zero);
    MeshComponent->SetEnableGravity(bInitialGravity);

    UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(GetRootComponent());
    
    if(!PrimitiveComponent) { return; }
    
    if(bInitialCollision)
    {
        PrimitiveComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
        PrimitiveComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
    }
    else
    {
        PrimitiveComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
        PrimitiveComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
    }

    PrimitiveComponent->SetMassScale(NAME_None, 1.f);
    
    bResetting = false;
}

