// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerupActor.generated.h"

UCLASS()
class PROJECTARCADE_API APowerupActor : public AActor
{
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  APowerupActor();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  //These are in case the powerp is for a certain period of time
  UPROPERTY(EditDefaultsOnly, Category = "Powerups")
    /*Time between powerup ticks*/
    float powerupInterval;
  /*Total times we apply the powerup effect*/
  int32 totalTicks;
  /*Total number of ticks applied*/
  int32 ticksProcessed;
  FTimerHandle TimerHandle_PowerupTick;
  UFUNCTION()
  void OnTickPowerup();

  UPROPERTY(BlueprintReadOnly)
    int32 playerNum;
public:
  void ActivatePowerup(AActor* player);

  UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
    void OnActivated(AActor* findMeInBlueprint);
  UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
    void OnExpired();
  UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
  void OnPowerUpTicked();
};
