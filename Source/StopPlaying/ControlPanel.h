#pragma once

#include "StopPlaying.h"

#include "InteractiveActor.h"
#include "ControlPanelButton.h"
#include "ControlPanelActorConnector.h"

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
     * Sets the appropriate panel title
     */
    void SetTitle();
    
    /**
     * Gets abstract active state
     */
    bool IsEffectActive(EControlPanelEffectType EffectType, float EffectScale);

    /**
     * Sets abstract active state
     */
    void SetEffectActive(EControlPanelEffectType EffectType, float EffectScale, bool bNewState);

    /**
     * Sets the connected actor
     */
    void SetConnectedActor(AActor* NewActor);

private:
    UPROPERTY(EditAnywhere)
    AActor* ConnectedActor = nullptr;
    
    // These will be automatically assigned
    UTextRenderComponent* TitleComponent = nullptr;
    TArray<UControlPanelActorConnector*> ActorConnectors;
    TArray<UControlPanelButton*> Buttons;

    /**
     * Initialises all widgets
     */
    void InitAllWidgets();

    /**
     * Updates all widgets
     */
    void UpdateAllWidgets();

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
     * Get global gravity state
     */
    bool GetGlobalGravity();

    /**
     * Set global gravity state
     */
    void SetGlobalGravity(bool bIsEnabled);
    
    /**
     * Get time state
     */
    bool GetTime(float EffectScale);

    /**
     * Set time state
     */
    void SetTime(bool bIsEnabled, float EffectScale);
    
    /**
     * Gets the mesh component of the connected actor
     */
    UMeshComponent* GetMeshComponent();

    /**
     * Gets a string based on effect type
     */
    FText EffectTypeToString(EControlPanelEffectType EffectType);
};
