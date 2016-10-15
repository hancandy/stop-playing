#pragma once

#include "InteractiveActor.h"

#include "Components/ChildActorComponent.h"
#include "ControlPanelWidget.generated.h"

UENUM(BlueprintType)
enum class EControlPanelWidgetType : uint8
{
    GRAVITY_BUTTON          UMETA(DisplayName="Gravity Button"),
    COLLISION_BUTTON        UMETA(DisplayName="Collision Button"),
    ROTATION_BUTTON         UMETA(DisplayName="Rotation Button")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STOPPLAYING_API UControlPanelWidget : public UChildActorComponent
{
	GENERATED_BODY()

public:
    UControlPanelWidget();

    /**
     * Widget tick
     */
    void TickWidget(float DeltaTime);

    /**
     * Gets abstract active state
     */
    bool IsEffectActive();

    /**
     * Sets abstract active state
     */
    void SetEffectActive(bool bNewState);

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
     * Initialises a widget
     */
    void Init(AActor* Connected);

private:
    UPROPERTY(EditAnywhere)
    EControlPanelWidgetType Type = EControlPanelWidgetType::GRAVITY_BUTTON;	
	
    UPROPERTY(EditAnywhere)
    FString Label = "Property";

    UPROPERTY(EditAnywhere)
    int32 Timeout = 0;

    UPROPERTY(EditAnywhere)
    bool bInitialState = false;

    UFUNCTION()
    void OnInteract(APawn* InteractingPawn);

    AActor* ConnectedActor = nullptr;
    AInteractiveActor* ChildActor = nullptr;
    
    FTimerHandle TimerHandle;
    int32 Timer = 0;

    bool bIsEffectActive = false;

    /**
     * Sets widget label
     */
    void SetLabel(FString Suffix = "");

    /**
     * Gets actor primitive component
     */
    UMeshComponent* GetActorMeshComponent();
 
    /**
     * Toggles the specified effect
     */
    void ToggleEffect();

    /**
     * Resets the timer
     */
    void ResetTimer();

    /**
     * Ticks the timer
     */
    void TickTimer();
    
    /**
     * Starts the timer
     */
    void StartTimer();
    
    /**
     * Checks if a connected actor is assigned
     */
    bool HasConnectedActor();

};
