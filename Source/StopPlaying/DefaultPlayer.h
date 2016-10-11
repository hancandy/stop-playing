// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractiveActor.h"

#include "GameFramework/Character.h"
#include "DefaultPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionPrompt, FString, Message);

UCLASS()
class STOPPLAYING_API ADefaultPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADefaultPlayer();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

private:
    UPROPERTY(EditAnywhere)
    bool bToggleInteraction = false;
    
    UPROPERTY(EditAnywhere)
    float LineTraceLength = 200.f;
    
    UPROPERTY(BlueprintAssignable, Category="DefaultPlayer")
    FOnInteractionPrompt OnInteractionPrompt;

    // Components
    void CheckComponents();

    UPhysicsHandleComponent* PhysicsHandle;

    // Locomotion
    void MoveForward(float AxisValue);
    void MoveRight(float AxisValue);
    void LookPitch(float AxisValue);
    void LookYaw(float AxisValue);
    void Jump();
	
    // Interaction
    void Interact();
    void StopInteract();
    void UpdateInteractionPrompt();
    void UpdateGrabbedComponent();
    void GetLineTrace(FVector& Begin, FVector& End);
    const FHitResult GetFirstPhysicsBodyInReach();
    AInteractiveActor* GetFirstInteractiveActorInReach();
};
