// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameMapsSettings.h"
#include "ArcadeGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARCADE_API UArcadeGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
    /**
      * Allow to set if keyboard and gamepad should be associate to same player
      */
    UFUNCTION(BlueprintCallable, Category = "Controllers")
    void AssociateGamepadToKeyboard(bool ShouldBeAssociated);
};
