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
     * Initialises a widget
     */
    void Init();

    /**
     * Gets the AInteractiveActor
     */
    AInteractiveActor* GetInteractiveActor();

    /**
     * Event: On update widget appearance
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "ControlPanelWidget")
    void OnUpdateWidgetAppearance(bool bWidgetIsActive);

protected:
    AInteractiveActor* ChildActor = nullptr;

};
