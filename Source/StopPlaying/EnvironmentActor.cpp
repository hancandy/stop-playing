

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
    TargetTransform = InitialTransform;
    
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
    
    if(RepeatResetIterations > 0)
    {
        RepeatResetIterations--;
        
        Reset();
    }

    TickTransform(DeltaTime);
}

void AEnvironmentActor::SetTargetRotation(FRotator NewRotation)
{
    RotationTimer = 1.f;

    TargetTransform.SetRotation(NewRotation.Quaternion());
}

void AEnvironmentActor::SetTargetScale(FVector NewScale)
{
    ScaleTimer = 1.f;

    TargetTransform.SetScale3D(NewScale);
}

void AEnvironmentActor::SetTargetLocation(FVector NewLocation)
{
    TranslationTimer = 1.f;

    TargetTransform.SetLocation(NewLocation);
}

void AEnvironmentActor::TickTransform(float DeltaTime)
{
    // Tick translation
    if(TranslationTimer != 0.f)
    {
        if(TranslationTimer < 0.f) { TranslationTimer = 0.f; }
        
        FVector NewLocation = FMath::Lerp(GetActorLocation(), TargetTransform.GetLocation(), 1.f - TranslationTimer);

        SetActorLocation(NewLocation);  

        TranslationTimer -= DeltaTime * 0.5f;
    }

    // Tick scale
    if(ScaleTimer != 0.f)
    {
        if(ScaleTimer < 0.f) { ScaleTimer = 0.f; }
        
        FVector NewScale = FMath::Lerp(GetActorScale3D(), TargetTransform.GetScale3D(), 1.f - ScaleTimer);

        SetActorScale3D(NewScale);  

        ScaleTimer -= DeltaTime * 0.5f;
    }

    // Tick rotation
    if(RotationTimer != 0.f)
    {
        if(RotationTimer < 0.f) { RotationTimer = 0.f; }

        FRotator NewRotation = FMath::Lerp(GetActorRotation(), TargetTransform.Rotator(), 1.f - RotationTimer);

        SetActorRotation(NewRotation);  

        RotationTimer -= DeltaTime * 0.5f;
    }
}

void AEnvironmentActor::Reset()
{
    Super::Reset();
    
    SetActorTransform(InitialTransform);
    
    TargetTransform = InitialTransform;

    UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(GetRootComponent());
    
    if(!PrimitiveComponent) { return; }
    
    for(auto& Elem : InitialCollision)
    {
        PrimitiveComponent->SetCollisionResponseToChannel(Elem.Key, Elem.Value);
    }
}

void AEnvironmentActor::RepeatReset(int32 Iterations)
{
    RepeatResetIterations = Iterations;
}

void AEnvironmentActor::Toggle(bool bIsEnabled)
{
    OnToggle.Broadcast(bIsEnabled);
}
