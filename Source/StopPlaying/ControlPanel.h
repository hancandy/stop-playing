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

    /**
     * Get gravity state
     */
    UFUNCTION(BlueprintCallable, Category="Control Panel")
    bool GetGravity();

    /**
     * Set gravity state
     */
    UFUNCTION(BlueprintCallable, Category="Control Panel")
    void SetGravity(bool bIsEnabled);
    
    /**
     * Get collision state
     */
    UFUNCTION(BlueprintCallable, Category="Control Panel")
    bool GetCollision();

    /**
     * Set collision state
     */
    UFUNCTION(BlueprintCallable, Category="Control Panel")
    void SetCollision(bool bIsEnabled);

    /**
     * Checks if we have a connected actor
     */
    UFUNCTION(BlueprintCallable, Category="Control Panel")
    bool HasConnectedActor();

private:
    UPROPERTY(EditAnywhere)
    bool bConnectedActorIsPlayer = false;

    UPROPERTY(EditAnywhere)
    AActor* ConnectedActor = nullptr;

    // These will be automatically assigned
    UTextRenderComponent* TitleComponent = nullptr;
    AInteractiveActor* GravityButton = nullptr;
    AInteractiveActor* CollisionButton = nullptr;

    /**
     * Assigns all widgets
     */
    void AssignAllWidgets();

    /**
     * Initialises all widgets with the correct labels
     */
    void InitAllWidgets();

    /**
     * Sets widget text
     */
    void SetWidgetText(AInteractiveActor* Widget, FString NewText);

    /**
     * Gets actor primiteive component
     */
    UPrimitiveComponent* GetActorPrimitiveComponent();

    /**
     * Sets the appropriate panel title
     */
    void SetTitle();

    /**
     * Initialises a widget
     */
    void InitWidget(UControlPanelWidget* Widget);

    /**
     * Event: Click gravity button
     */
    UFUNCTION()
    void OnClickGravityButton(APawn* InteractingPawn);
    
    /**
     * Event: Click collision button
     */
    UFUNCTION()
    void OnClickCollisionButton(APawn* InteractingPawn);
};
