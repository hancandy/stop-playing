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
	// Sets default values for this actor's properties
	AInteractiveActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    UPROPERTY(EditAnywhere)
    bool bCanBeGrabbed = true;

    UPROPERTY(EditAnywhere)
    FString InteractionMessage = "Interact";

    void Interact(APawn* InteractingPawn);
    
    UPROPERTY(BlueprintAssignable, Category="InteractiveActor")
    FOnInteraction OnInteraction;
	
private:

};
