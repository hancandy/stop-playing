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

    InitialPosition = GetActorLocation();
    InitialRotation = GetActorRotation();
    InitialScale = GetActorRelativeScale3D();    
}

// Called every frame
void AInteractiveActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AInteractiveActor::Interact(APawn* InteractingPawn)
{
    OnInteraction.Broadcast(InteractingPawn);
}

void AInteractiveActor::Reset()
{
    UE_LOG(LogTemp, Warning, TEXT("%s was reset"), *GetName());

    FVector Zero;

    UMeshComponent* MeshComponent = FindComponentByClass<UMeshComponent>();

    if(MeshComponent)
    {
        MeshComponent->SetPhysicsLinearVelocity(Zero);
        MeshComponent->SetPhysicsAngularVelocity(Zero);
    }
    
    SetActorLocation(InitialPosition);
    SetActorRotation(InitialRotation);
    SetActorRelativeScale3D(InitialScale);
}
