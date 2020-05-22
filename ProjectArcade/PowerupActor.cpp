// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupActor.h"
#include "TimerManager.h"
#include "ProjectArcadeCharacter.h"

// Sets default values
APowerupActor::APowerupActor()
{
  powerupInterval = 0.0f;
  totalTicks = 0.0f;
  playerNum = 0;
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
  AProjectArcadeCharacter* getplayer = Cast<AProjectArcadeCharacter>(player);
  playerNum = getplayer->playerID;
  if (playerNum == 1)
    playerNum = 0;
  else
    playerNum = 1;

  OnActivated(player);

  if (powerupInterval > 0)
    GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &APowerupActor::OnTickPowerup, powerupInterval, true);
  else
    OnTickPowerup();
}

