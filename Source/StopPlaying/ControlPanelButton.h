#pragma once

#include "StopPlaying.h"

#include "ControlPanelWidget.h"
#include "ControlPanelButton.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STOPPLAYING_API UControlPanelButton : public UControlPanelWidget
{
	GENERATED_BODY()
    
public:	
    /**
     * Event: On interact
     */
    UFUNCTION()
    virtual void OnInteract(APawn* InteractingPawn) override;

    UPROPERTY(EditAnywhere)
    EControlPanelEffectType EffectType = EControlPanelEffectType::GRAVITY;	
	
    UPROPERTY(EditAnywhere)
    float EffectScale = 1.f;

protected:
    UPROPERTY(EditAnywhere)
    int32 Timeout = 0;

    FTimerHandle TimerHandle;
    int32 Timer = 0;

    /**
     * Toggles the effect on/off
     */
    void ToggleEffect();

    /**
     * Sets the effect state
     */
    void SetEffectActive(bool bIsEffectActive);

    /**
     * Gets the effect state
     */
    bool IsEffectActive();

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

};