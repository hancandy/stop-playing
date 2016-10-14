#pragma once

#include "Components/ChildActorComponent.h"
#include "ControlPanelWidget.generated.h"

UENUM(BlueprintType)
enum class EControlPanelWidgetType : uint8
{
    GRAVITY_BUTTON          UMETA(DisplayName="Gravity Button"),
    COLLISION_BUTTON        UMETA(DisplayName="Collision Button")
};


UCLASS()
class STOPPLAYING_API UControlPanelWidget : public UChildActorComponent
{
	GENERATED_BODY()

public:
    UControlPanelWidget();

    EControlPanelWidgetType Type = EControlPanelWidgetType::GRAVITY_BUTTON;	
	
};
