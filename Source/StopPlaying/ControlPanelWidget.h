#pragma once

#include "InteractiveActor.h"

#include "Components/ChildActorComponent.h"
#include "ControlPanelWidget.generated.h"

UENUM(BlueprintType)
enum class EControlPanelWidgetType : uint8
{
    GRAVITY_BUTTON          UMETA(DisplayName="Gravity Button"),
    COLLISION_BUTTON        UMETA(DisplayName="Collision Button")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STOPPLAYING_API UControlPanelWidget : public UChildActorComponent
{
	GENERATED_BODY()

public:
    /**
     * Get gravity state
     */
    bool GetGravity();

    /**
     * Set gravity state
     */
    void SetGravity(bool bIsEnabled);
    
    /**
     * Get collision state
     */
    bool GetCollision();

    /**
     * Set collision state
     */
    void SetCollision(bool bIsEnabled);

    /**
     * Checks if we have a connected actor
     */
    bool HasConnectedActor();

    /**
     * Initialises a widget
     */
    void Init(AActor* Connected);

private:
    /**
     * Sets widget label
     */
    void SetLabel();

    /**
     * Gets actor primitive component
     */
    UPrimitiveComponent* GetActorPrimitiveComponent();
    
    UPROPERTY(EditAnywhere)
    EControlPanelWidgetType Type = EControlPanelWidgetType::GRAVITY_BUTTON;	
	
    UPROPERTY(EditAnywhere)
    FString Label = "Property";

    UFUNCTION()
    void OnInteract(APawn* InteractingPawn);

    AActor* ConnectedActor = nullptr;
    AInteractiveActor* ChildActor = nullptr;

};
