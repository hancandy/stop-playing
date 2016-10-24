#pragma once

#include "InteractiveActor.h"

#include "Components/ChildActorComponent.h"
#include "ControlPanelWidget.generated.h"

UCLASS()
class STOPPLAYING_API UControlPanelWidget : public UChildActorComponent
{
	GENERATED_BODY()

public:
    UControlPanelWidget();

    /**
     * Event: On interact
     */
    UFUNCTION()
    virtual void OnInteract(APawn* InteractingPawn);

    /**
     * Update widget state
     */
    void UpdateWidgetState(bool bWidgetIsActive);

    /**
     * Initialises a widget
     */
    virtual void Init();

    /**
     * Gets the AInteractiveActor
     */
    AInteractiveActor* GetInteractiveActor();

    /**
     * Sets the label
     */
    void SetLabel(FString NewLabel);

protected:
    AInteractiveActor* ChildActor = nullptr;

};
