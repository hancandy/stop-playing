

#include "StopPlaying.h"
#include "EnvironmentActor.h"


// Sets default values
AEnvironmentActor::AEnvironmentActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnvironmentActor::BeginPlay()
{
	Super::BeginPlay();
	
    InitialTransform = GetTransform();
    
    UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(GetRootComponent());
    
    if(!PrimitiveComponent)
    {
            TArray<USceneComponent*> SceneComponents;
            
            GetRootComponent()->GetChildrenComponents(true, SceneComponents);
            
            for(USceneComponent* SceneComponent : SceneComponents)
            {
                UPrimitiveComponent* ChildPrimitiveComponent = Cast<UPrimitiveComponent>(SceneComponent);
                
                if(ChildPrimitiveComponent)
                {
                    PrimitiveComponent = ChildPrimitiveComponent;
                    break;
                }
            }
    }
    
    if(!PrimitiveComponent) {
        UE_LOG(LogTemp, Error, TEXT("%s has no UPrimitiveComponent!"), *GetName());
        return;
    }

    bInitialCollision = PrimitiveComponent->GetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody) == ECollisionResponse::ECR_Block;
}

// Called every frame
void AEnvironmentActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AEnvironmentActor::Reset()
{
    SetActorTransform(InitialTransform);
    
    UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(GetRootComponent());
    
    if(!PrimitiveComponent) { return; }
    
    if(bInitialCollision)
    {
        PrimitiveComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
        PrimitiveComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
        PrimitiveComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
    }
    else
    {
        PrimitiveComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
        PrimitiveComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
        PrimitiveComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
    }

}

void AEnvironmentActor::Toggle(bool bIsEnabled)
{
    OnToggle.Broadcast(bIsEnabled);
}
