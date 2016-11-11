

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
    
    InitialCollision.Add(ECollisionChannel::ECC_PhysicsBody, PrimitiveComponent->GetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody));
    InitialCollision.Add(ECollisionChannel::ECC_WorldStatic, PrimitiveComponent->GetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic));
    InitialCollision.Add(ECollisionChannel::ECC_Pawn, PrimitiveComponent->GetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn));
}

// Called every frame
void AEnvironmentActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    if(RepeatReset > 0)
    {
        Reset();
        
        RepeatReset--;
    }
}

void AEnvironmentActor::Reset(bool bShouldRepeat)
{
    if(bShouldRepeat)
    {
        RepeatReset = 4;
    }
    
    SetActorTransform(InitialTransform);
    
    UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(GetRootComponent());
    
    if(!PrimitiveComponent) { return; }
    
    for(auto& Elem : InitialCollision)
    {
        PrimitiveComponent->SetCollisionResponseToChannel(Elem.Key, Elem.Value);
    }
}

void AEnvironmentActor::Toggle(bool bIsEnabled)
{
    OnToggle.Broadcast(bIsEnabled);
}
