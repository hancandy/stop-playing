// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractiveActor.h"
#include "NinjaCharacter.h"

#include "GameFramework/Character.h"
#include "DefaultPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionPrompt, FString, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetGravityScale, float, NewGravityScale);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGrabActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReleaseActor);

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
    UPROPERTY(BlueprintAssignable, Category="DefaultPlayer")
    FOnInteractionPrompt OnInteractionPrompt;
    
    UPROPERTY(BlueprintAssignable, Category="DefaultPlayer")
    FOnSetGravityScale OnSetGravityScale;
    
    UPROPERTY(BlueprintAssignable, Category="DefaultPlayer")
    FOnGrabActor OnGrabActor;
    
    UPROPERTY(BlueprintAssignable, Category="DefaultPlayer")
    FOnReleaseActor OnReleaseActor;
    
    UPROPERTY(EditAnywhere)
    float PushingPower = 10.f;

    UPROPERTY(EditAnywhere)
    float LineTraceLength = 200.f;
    
    UPROPERTY(EditAnywhere)
    float GrabbedActorDistance = 150.f;
    
    // Components
    void CheckComponents();

    UPhysicsHandleComponent* PhysicsHandle;

    // Locomotion
    void LookPitch(float AxisValue);
    void LookYaw(float AxisValue);
	
    // Interaction
    void Push();
    void GrabActor();
    void ReleaseActor();
    void UpdateInteractionPrompt();
    void UpdateGrabbedComponent();
    void GetLineTrace(FVector& Begin, FVector& End, bool bUseGrabDistance = false);
    const FHitResult GetFirstPhysicsBodyInReach();
    AInteractiveActor* GetFirstInteractiveActorInReach();
};
