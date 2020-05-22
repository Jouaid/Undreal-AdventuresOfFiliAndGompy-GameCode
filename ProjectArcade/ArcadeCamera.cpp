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

  //Get the players and assign them
  //playerOne = Cast<AProjectArcadeCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
  //playerTwo = Cast<AProjectArcadeCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
  //UE_LOG(LogTemp, Warning, TEXT("CALLLLLLLLLLLLLLLLLLLLED"));

  //setting up camera
  myCamera = UGameplayStatics::GetPlayerController(this, 0);

  MoveCamera();
}

// Called every frame
void AArcadeCamera::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  MoveCamera();
}

// Called to bind functionality to input
void AArcadeCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);
  myPlayerInputComponent = PlayerInputComponent;

  //Setting up players input
  //Player one
  PlayerInputComponent->BindAction("Jump", IE_Pressed, playerOne, &ACharacter::Jump);
  PlayerInputComponent->BindAction("Jump", IE_Released, playerOne, &ACharacter::StopJumping);
  PlayerInputComponent->BindAxis("OneForward", playerOne, &AProjectArcadeCharacter::MoveForward);
  PlayerInputComponent->BindAxis("OneRight", playerOne, &AProjectArcadeCharacter::MoveRight);
  //PlayerTwo
  PlayerInputComponent->BindAction("JumpTwo", IE_Pressed, playerTwo, &ACharacter::Jump);
  PlayerInputComponent->BindAction("JumpTwo", IE_Released, playerTwo, &ACharacter::StopJumping);
  PlayerInputComponent->BindAxis("TwoForward", playerTwo, &AProjectArcadeCharacter::MoveForward);
  PlayerInputComponent->BindAxis("TwoRight", playerTwo, &AProjectArcadeCharacter::MoveRight);
}

void AArcadeCamera::MoveCamera()
{
  //Null check
  if (playerOne != nullptr && playerTwo != nullptr)
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
      this->SetActorLocation(pos);
      //Moving the camera there
      myCamera->SetViewTargetWithBlend(actorToMoveTo, 4);
    }
  }
}

AProjectArcadeCharacter* AArcadeCamera::GetPlayerOne()
{
  if (playerOne != nullptr)
    return playerOne;

  return playerOne = Cast<AProjectArcadeCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

AProjectArcadeCharacter* AArcadeCamera::GetPlayerTwo()
{
  if (playerTwo != nullptr)
    return playerTwo;

  return playerTwo = Cast<AProjectArcadeCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void AArcadeCamera::SetPlayers(AProjectArcadeCharacter* getPlayerOne, AProjectArcadeCharacter* getPlayerTwo)
{
  this->playerOne = getPlayerOne;
  this->playerTwo = getPlayerTwo;
  UE_LOG(LogTemp, Warning, TEXT("PLAYER =  "), playerOne);
  UE_LOG(LogTemp, Warning, TEXT("PLAYER =  "), playerTwo);
}

