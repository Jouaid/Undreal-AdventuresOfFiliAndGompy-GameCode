// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadeGameViewportClient.h"

bool UArcadeGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
  //if (IgnoreInput() || EventArgs.IsGamepad() || EventArgs.Key.IsMouseButton())
  //{
  //  return Super::InputKey(Viewport, EventArgs.ControllerId, EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed, EventArgs.IsGamepad());
  //}
  //else
  //{
  //  // Propagate keyboard events to all players
  //  UEngine* const Engine = GetOuterUEngine();
  //  int32 const NumPlayers = Engine ? Engine->GetNumGamePlayers(this) : 0;
  //  bool bRetVal = false;
  //  for (int32 i = 0; i < NumPlayers; i++)
  //  {
  //    bRetVal = Super::InputKey(Viewport, i, EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed, EventArgs.IsGamepad()) || bRetVal;
  //  }

  //  return bRetVal;
  //}

  //if (IgnoreInput() || EventArgs.IsGamepad() || EventArgs.Key.IsMouseButton())
  //{
  //  return Super::InputKey(EventArgs);
  //}
  //else
  //{
  //  // Propagate keyboard events to all players
  //  UEngine* const Engine = GetOuterUEngine();
  //  int32 const NumPlayers = Engine ? Engine->GetNumGamePlayers(this) : 0;
  //  bool bRetVal = false;
  //  for (int32 i = 0; i < NumPlayers; i++)
  //    bRetVal = Super::InputKey(EventArgs) || bRetVal;

  //  return bRetVal;
  //}

  UEngine* const Engine = GetOuterUEngine();
  int32 const NumPlayers = Engine ? Engine->GetNumGamePlayers(this) : 0;
  bool bRetVal = false;
  for (int32 i = 0; i < NumPlayers; i++)
    bRetVal = Super::InputKey(EventArgs) || bRetVal;

  return bRetVal;

  //UEngine* const Engine = GetOuterUEngine();
  //int32 const NumPlayers = Engine ? Engine->GetNumGamePlayers(this) : 0;
  //bool returnValue = false;
  //for (int32 i = 0; i < NumPlayers; i++)
  //{
  //  returnValue = Super::InputKey(Viewport, i, EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed, EventArgs.IsGamepad()) || returnValue;
  //}

  //return returnValue;
}

bool UArcadeGameViewportClient::InputAxis(FViewport* tViewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
    // Map the gamepad to the next player index (so 1st controller is player index 1, etc.)
    return Super::InputAxis(Viewport, ControllerId + 1, Key, Delta, DeltaTime, NumSamples, bGamepad);
}
