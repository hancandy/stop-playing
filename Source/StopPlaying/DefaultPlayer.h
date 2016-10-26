// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractiveActor.h"
#include "NinjaCharacter.h"

#include "GameFramework/Character.h"
#include "DefaultPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionPrompt, FString, Message);

UCLASS()
class STOPPLAYING_API ADefaultPlayer : public ANinjaCharacter
{
	GENERATED_BODY()

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    void SetGravityScale(float NewGravityScale);
    float GetGravityScale();

private:
    UPROPERTY(EditAnywhere)
    float LineTraceLength = 200.f;
    
    UPROPERTY(BlueprintAssignable, Category="DefaultPlayer")
    FOnInteractionPrompt OnInteractionPrompt;
    
    UPROPERTY(EditAnywhere)
    float PushingPower = 10.f;

    // Components
    void CheckComponents();

    UPhysicsHandleComponent* PhysicsHandle;

    // Locomotion
    void LookPitch(float AxisValue);
    void LookYaw(float AxisValue);
	
    // Interaction
    void Push();
    void Grab();
    void StopGrab();
    void UpdateInteractionPrompt();
    void UpdateGrabbedComponent();
    void GetLineTrace(FVector& Begin, FVector& End);
    const FHitResult GetFirstPhysicsBodyInReach();
    AInteractiveActor* GetFirstInteractiveActorInReach();
};
