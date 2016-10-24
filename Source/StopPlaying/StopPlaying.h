// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"

UENUM(BlueprintType)
enum class EControlPanelEffectType : uint8
{
    GRAVITY          UMETA(DisplayName="Gravity"),
    COLLISION        UMETA(DisplayName="Collision"),
    ROTATION         UMETA(DisplayName="Rotation"),
    TRANSLATION      UMETA(DisplayName="Translation"),
    SCALE            UMETA(DisplayName="Scale"),
    WORLD_TIME       UMETA(DisplayName="World time"),
    WORLD_GRAVITY    UMETA(DisplayName="World gravity")
};

