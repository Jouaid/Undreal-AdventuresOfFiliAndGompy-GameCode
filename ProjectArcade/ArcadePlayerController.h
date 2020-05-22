// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectArcadeCharacter.h"
#include "ArcadePlayerController.generated.h"

/**
 *
 */
UCLASS()
class PROJECTARCADE_API AArcadePlayerController : public APlayerController
{
  GENERATED_BODY()


protected:
  UPROPERTY(EditAnywhere,Category = "Player")
    TSubclassOf<AProjectArcadeCharacter> customPlayerOne;
  UPROPERTY(EditAnywhere, Category = "Player")
  TSubclassOf<AProjectArcadeCharacter> customPlayerTwo;
  virtual void SetupInputComponent() override;
};
