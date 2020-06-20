// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupActor.h"
#include "TimerManager.h"
#include "ProjectArcadeCharacter.h"

// Sets default values
APowerupActor::APowerupActor()
{
  powerupInterval = 0.0f;
  totalTicks = 0.0f;
}

// Called when the game starts or when spawned
void APowerupActor::BeginPlay()
{
  Super::BeginPlay();
}

void APowerupActor::OnTickPowerup()
{
  //Increment
  ticksProcessed++;
  //Call the powerup
  OnPowerUpTicked();

  if (ticksProcessed >= totalTicks)
  {
    //Remove the powerup
    OnExpired();

    //Delete timer
    GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
  }
}

void APowerupActor::ActivatePowerup(AActor* player)
{
  OnActivated(player);

  if (powerupInterval > 0)
    GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &APowerupActor::OnTickPowerup, powerupInterval, true);
  else
    OnTickPowerup();
}

