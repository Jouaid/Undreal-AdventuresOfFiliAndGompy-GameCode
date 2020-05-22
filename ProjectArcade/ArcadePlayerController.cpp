// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadePlayerController.h"
#include "ProjectArcadeCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void AArcadePlayerController::SetupInputComponent()
{
  //UWorld* world = GetWorld();

  //TArray<AActor*> playerStarts;

  //UGameplayStatics::GetAllActorsOfClass(world, APlayerStart::StaticClass(), playerStarts);

  //for (size_t i = 0; i < 2; i++)
  //{
    //AProjectArcadeCharacter* player = world->SpawnActor<AProjectArcadeCharacter>(customPlayerOne->StaticClass(),
                                                        //playerStarts[i]->GetTransform().GetLocation(),
                                                        //playerStarts[i]->GetTransform().GetRotation().Rotator());
  //}

  /*int32 id = GetLocalPlayer()->GetControllerId();
  dynamic_cast<AProjectArcadeCharacter*>(GetPawn());
  if (id == 0)
  {
    InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    InputComponent->BindAxis("OneForward", this, &AArcadePlayerController::MoveForward);
    InputComponent->BindAxis("OneRight", this, &AArcadePlayerController::MoveRight);
  }
  else if (id == 1)
  {
    InputComponent->BindAction("JumpTwo", IE_Pressed, this, &AArcadePlayerController::Jump);
    InputComponent->BindAction("JumpTwo", IE_Released, this, &AArcadePlayerController::StopJumping);
    InputComponent->BindAxis("TwoForward", this, &AArcadePlayerController::MoveForward);
    InputComponent->BindAxis("TwoRight", this, &AArcadePlayerController::MoveRight);
  }*/
}