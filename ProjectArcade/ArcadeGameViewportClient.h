// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Engine.h"
#include "ArcadeGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARCADE_API UArcadeGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()

protected:
  virtual bool InputKey(const FInputKeyEventArgs& EventArgs) override;

  virtual bool InputAxis(FViewport* tViewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples = 1, bool bGamepad = false) override;
};
