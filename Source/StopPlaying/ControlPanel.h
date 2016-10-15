#pragma once

#include "InteractiveActor.h"
#include "ControlPanelWidget.h"

#include "GameFramework/Actor.h"
#include "ControlPanel.generated.h"

UCLASS()
class STOPPLAYING_API AControlPanel : public AActor
{
	GENERATED_BODY()

public:	
	AControlPanel();

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

private:
    UPROPERTY(EditAnywhere)
    bool bConnectedActorIsWorld = false;

    UPROPERTY(EditAnywhere)
    AActor* ConnectedActor = nullptr;

    // These will be automatically assigned
    UTextRenderComponent* TitleComponent = nullptr;
    TArray<UControlPanelWidget*> Widgets;

    /**
     * Initialises all widgets with the correct labels
     */
    void InitAllWidgets();

    /**
     * Sets the appropriate panel title
     */
    void SetTitle();
};
