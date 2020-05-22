// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadeGameInstance.h"
#include "UObject/Class.h"

void UArcadeGameInstance::AssociateGamepadToKeyboard(bool ShouldBeAssociated)
{
  // Get GameSettings
  UGameMapsSettings* Settings = const_cast<UGameMapsSettings*>(GetDefault<UGameMapsSettings>());

  // It will affect Gamepad from Controller 1 (and keyboard to 1) if they should not be associated
  Settings->bOffsetPlayerGamepadIds = !ShouldBeAssociated;
}