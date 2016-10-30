

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
	
}

// Called every frame
void AEnvironmentActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AEnvironmentActor::Toggle(bool bIsEnabled)
{
    OnToggle.Broadcast(bIsEnabled);
}
