#pragma once

#include "EnvironmentActor.h"

#include "GameFramework/Actor.h"
#include "InteractiveActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteraction, APawn*, InteractingPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginReset);

UCLASS()
class STOPPLAYING_API AInteractiveActor : public AEnvironmentActor
{
	GENERATED_BODY()
	
public:	
	AInteractiveActor();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

    UPROPERTY(EditAnywhere)
    bool bCanBeGrabbed = true;

    UPROPERTY(EditAnywhere)
    FString InteractionMessage = "Interact";

    UPROPERTY(BlueprintAssignable, Category="InteractiveActor")
    FOnInteraction OnInteraction;
    
    UPROPERTY(BlueprintAssignable, Category="InteractiveActor")
    FOnBeginReset OnBeginReset;
   
    bool bResetting = false;

    /**
     * Interacts with this actor
     */
    void Interact(APawn* InteractingPawn);
   
    /**
     * Begins resetting this actor
     */
    void BeginReset();

    /**
     * Resets this actor
     */
    virtual void Reset() override;

protected:
    bool bInitialGravity = false;

};
