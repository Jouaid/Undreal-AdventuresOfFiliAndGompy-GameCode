// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadeCamera.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AArcadeCamera::AArcadeCamera()
{
  // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  // Create a camera boom (pulls in towards the player if there is a collision)
  CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
  CameraBoom->SetupAttachment(RootComponent);
  CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
  CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

  // Create a follow camera
  FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
  FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
  FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

// Called when the game starts or when spawned
void AArcadeCamera::BeginPlay()
{
  Super::BeginPlay();

  myCamera = UGameplayStatics::GetPlayerController(this, 0);
  splitCamera = false;

  MoveCamera();
}

// Called every frame
void AArcadeCamera::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  MoveCamera();
}

void AArcadeCamera::MoveCamera()
{
  //Null check
  if (playerOne != nullptr && playerTwo != nullptr)
  {

    floatDist = dist.Distance(playerOne->GetActorLocation(), playerTwo->GetActorLocation());
    //spliting and joining camera logic
    if (floatDist > distCheckNumber && !splitCamera)
    {
      splitCamera = true;
      SplitCamera(this);
    }
    else if (floatDist < distCheckNumber && splitCamera)
    {
      splitCamera = false;
      JoinCamera(this);
    }

    if (!splitCamera)
    {
      //calculating the distance between the two objects
      dist = playerOne->GetActorLocation() - playerTwo->GetActorLocation();
      //Null check
      if (actorToMoveTo != nullptr)
      {
        //Calculating Position
        FVector pos;
        pos = dist / 2;
        pos += playerTwo->GetActorLocation();

        //assigning that position to our empty actor which should only mark the position
        actorToMoveTo->SetActorLocation(FVector(pos.X - xOffSet, pos.Y, pos.Z + zOffSet));

        //Moving the camera there
        myCamera->SetViewTargetWithBlend(actorToMoveTo, 4);
      }
    }
  }
}

void AArcadeCamera::SetPlayers(AProjectArcadeCharacter* getPlayerOne, AProjectArcadeCharacter* getPlayerTwo)
{
  this->playerOne = getPlayerOne;
  this->playerTwo = getPlayerTwo;
}

