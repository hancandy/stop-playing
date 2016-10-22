// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ControlPanelWidget.h"
#include "ControlPanelActorConnector.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STOPPLAYING_API UControlPanelActorConnector : public UControlPanelWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    AActor* ConnectedActor = nullptr;    
	
    /**
     * Event: On interact
     */
    UFUNCTION()
    virtual void OnInteract(APawn* InteractingPawn) override;
	
	
};
