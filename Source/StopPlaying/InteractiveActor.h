// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "InteractiveActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteraction, APawn*, InteractingPawn);

UCLASS()
class STOPPLAYING_API AInteractiveActor : public AActor
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
	
    /**
     * Interacts with this actor
     */
    void Interact(APawn* InteractingPawn);
   
    /**
     * Resets this actor's transform
     */
    void Reset();

private:
    FVector InitialPosition;
    FRotator InitialRotation;
    FVector InitialScale;

};
