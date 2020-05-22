// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProjectArcadeCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "HealthComponent.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Spawner.h"

//////////////////////////////////////////////////////////////////////////
// AProjectArcadeCharacter

AProjectArcadeCharacter::AProjectArcadeCharacter()
{
  // Set size for collision capsule
  GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
  GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AProjectArcadeCharacter::OnOverlapBegin);

  // Don't rotate when the controller rotates. Let that just affect the camera.
  bUseControllerRotationPitch = false;
  bUseControllerRotationYaw = false;
  bUseControllerRotationRoll = false;

  // Configure character movement
  GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
  GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
  GetCharacterMovement()->JumpZVelocity = 600.f;
  GetCharacterMovement()->AirControl = 0.2f;

  // Create a camera boom (pulls in towards the player if there is a collision)
  CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
  CameraBoom->SetupAttachment(RootComponent);
  CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
  CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

  // Create a follow camera
  FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
  FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
  FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

  //setting up the hitbox where you can damage an enemy or player
  hitBox = CreateDefaultSubobject<USphereComponent>(TEXT("HitBox"));
  hitBox->SetSphereRadius(65);
  hitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
  hitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
  hitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
  hitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
  hitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Overlap);
  hitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
  hitBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectArcadeCharacter::OnOverlapBegin);
  hitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  hitBox->SetupAttachment(GetCapsuleComponent());

  //setting up the shield for the player
  shieldBox = CreateDefaultSubobject<USphereComponent>(TEXT("ShieldBox"));
  shieldBox->SetSphereRadius(125);
  shieldBox->SetCollisionResponseToAllChannels(ECR_Block);
  shieldBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  shieldBox->SetupAttachment(RootComponent);

  staticMeshShield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
  staticMeshShield->SetCollisionResponseToAllChannels(ECR_Ignore);
  staticMeshShield->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  staticMeshShield->SetRelativeScale3D(FVector(2.4f));
  staticMeshShield->SetSimulatePhysics(false);
  staticMeshShield->SetupAttachment(shieldBox);

  tailHitBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Tail HitBox"));
  tailHitBox->SetCapsuleSize(25, 12);
  tailHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
  tailHitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
  tailHitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
  tailHitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
  tailHitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Overlap);
  tailHitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
  tailHitBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectArcadeCharacter::OnOverlapBegin);
  tailHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  tailHitBox->SetupAttachment(RootComponent);

  smashHitBox = CreateDefaultSubobject<USphereComponent>(TEXT("Smash HitBox"));
  smashHitBox->SetSphereRadius(200);
  smashHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
  smashHitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
  smashHitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
  smashHitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
  smashHitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Overlap);
  smashHitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
  smashHitBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectArcadeCharacter::OnOverlapBegin);
  smashHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  smashHitBox->SetupAttachment(RootComponent);


  //BOSS HIT BOXESS
  bossTailHitBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Boss Tail HitBox"));
  bossTailHitBox->SetCapsuleSize(25, 12);
  bossTailHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
  bossTailHitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
  bossTailHitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
  bossTailHitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
  bossTailHitBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectArcadeCharacter::OnOverlapBegin);
  bossTailHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  bossTailHitBox->SetupAttachment(RootComponent);

  bossSmashHitBox = CreateDefaultSubobject<USphereComponent>(TEXT("Boss Smash HitBox"));
  bossSmashHitBox->SetSphereRadius(200);
  bossSmashHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
  bossSmashHitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
  bossSmashHitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
  bossSmashHitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
  bossSmashHitBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectArcadeCharacter::OnOverlapBegin);
  bossSmashHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  bossSmashHitBox->SetupAttachment(RootComponent);

  //Making the healthCommp
  cHealth = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

  dead = false;
  canBasicAttack = true;
  canShield = true;
  canSmashAttack = true;
  canTwistAttack = true;
  canDash = true;

  bossCanBasicAttack = true;
  bossSecondCanBasicAttack = true;
}

void AProjectArcadeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
  check(PlayerInputComponent);
  //UE_LOG(LogTemp, Warning, TEXT("SETUPPLAYERINPUT CALLED"));

  APlayerController* pc = Cast<APlayerController>(GetController());
  UWorld* world = GetWorld();

  playerID = 1;
  if (pc && world)
  {
    ULocalPlayer* localPlayer = pc->GetLocalPlayer();

    //MaybeTrun on??
    //localPlayer->GetPlayerController(world)->bAutoManageActiveCameraTarget = false;

    if (localPlayer)
    {
      int32 id = localPlayer->GetControllerId();

      if (id == 1)
      {
        UE_LOG(LogTemp, Warning, TEXT("Spawned player with %d"))
          return;
      }
      else if (id > 1)
      {
        // UE4 sometimes spawns a player with a ridiculously high number, set it to 0 if it does.
        localPlayer->SetControllerId(0);
        UE_LOG(LogTemp, Warning, TEXT("Spawned player with %d"))
      }
    }

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAxis("OneForward", this, &AProjectArcadeCharacter::MoveForward);
    PlayerInputComponent->BindAxis("OneRight", this, &AProjectArcadeCharacter::MoveRight);

    PlayerInputComponent->BindAction("P1Attack", IE_Pressed, this, &AProjectArcadeCharacter::DoAttack);

    PlayerInputComponent->BindAction("P1A1", IE_Pressed, this, &AProjectArcadeCharacter::CallShieldOnInput);
    PlayerInputComponent->BindAction("P1A2", IE_Pressed, this, &AProjectArcadeCharacter::CallSmashAttackOnInput);
    PlayerInputComponent->BindAction("P1A3", IE_Pressed, this, &AProjectArcadeCharacter::CallDashOnInput);

    //Player two
    PlayerInputComponent->BindAction("JumpTwo", IE_Pressed, this, &AProjectArcadeCharacter::Jump_P2);
    PlayerInputComponent->BindAction("JumpTwo", IE_Released, this, &AProjectArcadeCharacter::StopJump_P2);
    PlayerInputComponent->BindAxis("TwoForward", this, &AProjectArcadeCharacter::MoveForward_P2);
    PlayerInputComponent->BindAxis("TwoRight", this, &AProjectArcadeCharacter::MoveRight_P2);

    PlayerInputComponent->BindAction("P2Attack", IE_Pressed, this, &AProjectArcadeCharacter::DoAttack_P2);

    PlayerInputComponent->BindAction("P2A1", IE_Pressed, this, &AProjectArcadeCharacter::CallDashOnInput);
    PlayerInputComponent->BindAction("P2A2", IE_Pressed, this, &AProjectArcadeCharacter::CallTwistAttackOnInput);

    FActorSpawnParameters spawnParams;

    //spawnParams.Template = player2Template;
    spawnParams.Owner = this;

    FVector position;
    FRotator rotator;

    TArray<AActor*> playerStarts;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(),
      playerStarts);

    UE_LOG(LogTemp, Warning, TEXT("Players STARTS: %d"), playerStarts.Num());

    for (size_t i = 0; i < playerStarts.Num(); i++)
    {
      float distance = FVector::Distance(playerStarts[i]->GetActorLocation(), GetActorLocation());
      if (distance < 100)
      {
        UE_LOG(LogTemp, Warning, TEXT("Distance is %s"), *FString::SanitizeFloat(distance));
        continue;
      }

      position = playerStarts[i]->GetActorLocation();
      rotator = playerStarts[i]->GetActorRotation();
      player2 = GetWorld()->SpawnActor<AProjectArcadeCharacter>(player2Template, position, rotator, spawnParams);
      if (player2)
      {
        playerID = -1;
        player2->playerID = 2;
        player2->SpawnDefaultController();
        UE_LOG(LogTemp, Warning, TEXT("Spawned player with name: %s"), *player2->GetFName().ToString());
      }
    }
  }
}

void AProjectArcadeCharacter::BeginPlay()
{
  Super::BeginPlay();
  cHealth->Reset();
  cHealth->OnHealthChanged.AddDynamic(this, &AProjectArcadeCharacter::OnHealthChanged);
  staticMeshShield->SetVisibility(false);
  staticMeshShield->SetSimulatePhysics(false);
}

ASpawner* AProjectArcadeCharacter::GetSpwaner()
{
  if (spawner != nullptr)
  {
    return spawner;
    UE_LOG(LogTemp, Warning, TEXT("GOT SPAWNER"));
  }

  return nullptr;
}

void AProjectArcadeCharacter::AttackOne()
{
  OnTestDelegate.Broadcast();
}

void AProjectArcadeCharacter::AssignEventToFunction()
{
  OnTestDelegate.AddDynamic(this, &AProjectArcadeCharacter::TestFunction);
}

void AProjectArcadeCharacter::TestFunction()
{
  UE_LOG(LogTemp, Warning, TEXT("CALLED FUNCTION"));
}

void AProjectArcadeCharacter::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

void AProjectArcadeCharacter::MoveForward(float Value)
{
  if ((Controller != NULL) && (Value != 0.0f))
  {
    // find out which way is forward
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    // get forward vector
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(Direction, Value);
  }
}

void AProjectArcadeCharacter::MoveRight(float Value)
{
  if ((Controller != NULL) && (Value != 0.0f))
  {
    // find out which way is right
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    // get right vector 
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    // add movement in that direction
    AddMovementInput(Direction, Value);
  }
}

void AProjectArcadeCharacter::DoAttack()
{
  if (canBasicAttack)
  {
    SwapAttackBool();
    hitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    GetWorldTimerManager().SetTimer(TimerHandle_AttackTimer, this, &AProjectArcadeCharacter::SwapAttackBool, basicAttackCooldown, false, basicAttackCooldown);
    GetWorldTimerManager().SetTimer(TimerHandle_StopAttackTimer, this, &AProjectArcadeCharacter::StopAttack, basicAttackCooldown + 0.1f, false, basicAttackCooldown + 0.1f);
    UE_LOG(LogTemp, Warning, TEXT("HITBOX ONNN"));
  }
}
void AProjectArcadeCharacter::StopAttack()
{
  hitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  GetWorldTimerManager().ClearTimer(TimerHandle_AttackTimer);
  GetWorldTimerManager().ClearTimer(TimerHandle_StopAttackTimer);
  UE_LOG(LogTemp, Warning, TEXT("HITBOX OFF"));

}

void AProjectArcadeCharacter::DoAttack_P2()
{
  if (player2->canBasicAttack)
  {
    player2->SwapAttackBool();
    player2->hitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    player2->GetWorldTimerManager().SetTimer(player2->TimerHandle_AttackTimer, this, &AProjectArcadeCharacter::SwapAttackBool_P2, player2->basicAttackCooldown, false, player2->basicAttackCooldown);
    player2->GetWorldTimerManager().SetTimer(player2->TimerHandle_StopAttackTimer, this, &AProjectArcadeCharacter::StopAttack_P2, player2->basicAttackCooldown + 0.1f, false, player2->basicAttackCooldown + 0.1f);
    UE_LOG(LogTemp, Warning, TEXT("HITBOX ONNN"));
  }
}
void AProjectArcadeCharacter::StopAttack_P2()
{
  player2->hitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  player2->GetWorldTimerManager().ClearTimer(player2->TimerHandle_AttackTimer);
  player2->GetWorldTimerManager().ClearTimer(player2->TimerHandle_StopAttackTimer);
  UE_LOG(LogTemp, Warning, TEXT("HITBOX OFF"));

}

void AProjectArcadeCharacter::CallShieldOnInput()
{
  OnShieldDelegate.Broadcast();
}

void AProjectArcadeCharacter::DoShield()
{
  //ACTIVATE SHIELD
  if (canShield)
  {
    canShield = false;
    staticMeshShield->SetVisibility(true);
    shieldBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetWorldTimerManager().SetTimer(TimerHandle_ShieldTimer, this, &AProjectArcadeCharacter::SwapShieldBool, shieldCooldown + shieldCooldown, false, shieldCooldown + shieldCooldown);
    GetWorldTimerManager().SetTimer(TimerHandle_StopShieldTimer, this, &AProjectArcadeCharacter::StopShield, shieldCooldown, false, shieldCooldown);
  }

}

void AProjectArcadeCharacter::StopShield()
{
  //DISABLE SHIELD
  GetWorldTimerManager().ClearTimer(TimerHandle_StopShieldTimer);
  staticMeshShield->SetVisibility(false);
  shieldBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AProjectArcadeCharacter::AssignShiledToFunction()
{
  OnShieldDelegate.AddDynamic(this, &AProjectArcadeCharacter::DoShield);
}

void AProjectArcadeCharacter::Kill()
{
  dead = true;
  SetActorTickEnabled(false);
  SetActorEnableCollision(false);
  GetCharacterMovement()->GravityScale = 0;
  StopJumping();
  GetCharacterMovement()->StopMovementImmediately();
  GetCharacterMovement()->Deactivate();

  //Destroys character
  GetOwner()->SetLifeSpan(3.0f);
  Destroy();
}

bool AProjectArcadeCharacter::IsDead()
{
  return dead;
}

void AProjectArcadeCharacter::OnHealthChanged(UHealthComponent* HealthComp, float currentHealth, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{

}

void AProjectArcadeCharacter::SwapAttackBool_P2()
{
  player2->canBasicAttack = !player2->canBasicAttack;
}

void AProjectArcadeCharacter::SwapAttackBool()
{
  canBasicAttack = !canBasicAttack;
}

void AProjectArcadeCharacter::SwapShieldBool()
{
  GetWorldTimerManager().ClearTimer(TimerHandle_ShieldTimer);
  canShield = !canShield;
}

void AProjectArcadeCharacter::CallTwistAttackOnInput()
{
  player2->OnTwistAttackDelegate.Broadcast();
}

void AProjectArcadeCharacter::DoTwistAttack()
{
  if (player2->canTwistAttack)
  {
    UE_LOG(LogTemp, Warning, TEXT("TWISTIN'!"));

    player2->canTwistAttack = false;
    player2->tailHitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    player2->GetWorldTimerManager().SetTimer(player2->TimerHandle_TwistAttackTimer, this, &AProjectArcadeCharacter::SwapTwistAttackdBool, player2->twistAttackCooldown + player2->twistAttackCooldown, false, player2->twistAttackCooldown + player2->twistAttackCooldown);
    player2->GetWorldTimerManager().SetTimer(player2->TimerHandle_StopTwistAttackTimer, this, &AProjectArcadeCharacter::StopTwistAttack, player2->twistAttackCooldown, false, player2->twistAttackCooldown);
  }
}

void AProjectArcadeCharacter::StopTwistAttack()
{
  player2->GetWorldTimerManager().ClearTimer(TimerHandle_StopTwistAttackTimer);
  player2->tailHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AProjectArcadeCharacter::AssignTwistAttackToFunction()
{
  player2->OnTwistAttackDelegate.AddDynamic(this, &AProjectArcadeCharacter::DoTwistAttack);
}

void AProjectArcadeCharacter::SwapTwistAttackdBool()
{
  player2->GetWorldTimerManager().ClearTimer(player2->TimerHandle_TwistAttackTimer);
  player2->canTwistAttack = !player2->canTwistAttack;
}

void AProjectArcadeCharacter::CallSmashAttackOnInput()
{
  OnSmashAttackDelegate.Broadcast();
}

void AProjectArcadeCharacter::DoSmashAttack()
{

  if (canSmashAttack)
  {
    UE_LOG(LogTemp, Warning, TEXT("SMASHIN'!"));

    canSmashAttack = false;
    smashHitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetWorldTimerManager().SetTimer(TimerHandle_SmashAttackTimer, this, &AProjectArcadeCharacter::SwapSmashAttackdBool, SmashAttackCooldown + SmashAttackCooldown, false, SmashAttackCooldown + SmashAttackCooldown);
    GetWorldTimerManager().SetTimer(TimerHandle_StopSmashAttackTimer, this, &AProjectArcadeCharacter::StopSmashAttack, SmashAttackCooldown, false, SmashAttackCooldown);
  }
}

void AProjectArcadeCharacter::StopSmashAttack()
{
  GetWorldTimerManager().ClearTimer(TimerHandle_StopSmashAttackTimer);
  smashHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AProjectArcadeCharacter::AssignSmashAttackToFunction()
{
  OnSmashAttackDelegate.AddDynamic(this, &AProjectArcadeCharacter::DoSmashAttack);
}

void AProjectArcadeCharacter::SwapSmashAttackdBool()
{
  GetWorldTimerManager().ClearTimer(TimerHandle_SmashAttackTimer);
  canSmashAttack = !canSmashAttack;
}

void AProjectArcadeCharacter::CallDashOnInput()
{
  player2->OnDashDelegate.Broadcast();
}

void AProjectArcadeCharacter::DoDash()
{
  if (player2->canDash)
  {
    player2->canDash = false;
    player2->GetWorldTimerManager().SetTimer(player2->TimerHandle_DashTimer, this, &AProjectArcadeCharacter::SwapDashBool, player2->DashCooldown, false, player2->DashCooldown);
    player2->LaunchCharacter(player2->GetActorForwardVector() * player2->dashAmount, false, false);
  }
}

void AProjectArcadeCharacter::AssignDashToFunction()
{
  player2->OnDashDelegate.AddDynamic(this, &AProjectArcadeCharacter::DoDash);
}

void AProjectArcadeCharacter::SwapDashBool()
{
  player2->GetWorldTimerManager().ClearTimer(player2->TimerHandle_DashTimer);
  player2->canDash = !player2->canDash;
}

void AProjectArcadeCharacter::MoveRight_P2(float Value)
{
  if ((Controller != NULL) && (Value != 0.0f))
  {
    // find out which way is right
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    // get right vector 
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    // add movement in that direction
    player2->AddMovementInput(Direction, Value);
  }
}

void AProjectArcadeCharacter::MoveForward_P2(float Value)
{
  if ((Controller != NULL) && (Value != 0.0f))
  {
    // find out which way is forward
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    // get forward vector
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    player2->AddMovementInput(Direction, Value);
  }
}

void AProjectArcadeCharacter::Jump_P2()
{
  player2->Jump();
}

void AProjectArcadeCharacter::StopJump_P2()
{
  player2->StopJumping();
}

void AProjectArcadeCharacter::Heal(float amount)
{
  cHealth->Heal(amount);
}

void AProjectArcadeCharacter::BossAttack()
{
  if (bossCanBasicAttack)
  {
    UE_LOG(LogTemp, Warning, TEXT("SMASHIN'!"));

    bossCanBasicAttack = false;
    bossSmashHitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetWorldTimerManager().SetTimer(TimerHandle_BossAttackTimer, this, &AProjectArcadeCharacter::BossSwapAttackBool, bossBasicAttackCooldown + bossBasicAttackCooldown, false, bossBasicAttackCooldown + bossBasicAttackCooldown);
    GetWorldTimerManager().SetTimer(TimerHandle_BossStopAttackTimer, this, &AProjectArcadeCharacter::BossStopAttack, bossBasicAttackCooldown, false, bossBasicAttackCooldown);
  }
}

void AProjectArcadeCharacter::BossStopAttack()
{
  bossSmashHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  GetWorldTimerManager().ClearTimer(TimerHandle_BossAttackTimer);
  GetWorldTimerManager().ClearTimer(TimerHandle_BossStopAttackTimer);
}

void AProjectArcadeCharacter::BossSwapAttackBool()
{
  bossCanBasicAttack = !bossCanBasicAttack;

}

void AProjectArcadeCharacter::BossSecondAttack()
{
  if (bossSecondCanBasicAttack)
  {
    UE_LOG(LogTemp, Warning, TEXT("BOSS 2nD ATTACK'!"));

    bossSecondCanBasicAttack = false;
    bossTailHitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetWorldTimerManager().SetTimer(TimerHandle_BossSecondAttackTimer, this, &AProjectArcadeCharacter::BossSecondSwapAttackBool, bossSecondBasicAttackCooldown + bossSecondBasicAttackCooldown, false, bossSecondBasicAttackCooldown + bossSecondBasicAttackCooldown);
    GetWorldTimerManager().SetTimer(TimerHandle_BossSecondStopAttackTimer, this, &AProjectArcadeCharacter::BossSecondStopAttack, bossSecondBasicAttackCooldown, false, bossSecondBasicAttackCooldown);
  }
}

void AProjectArcadeCharacter::BossSecondStopAttack()
{
  bossTailHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  GetWorldTimerManager().ClearTimer(TimerHandle_BossSecondAttackTimer);
  GetWorldTimerManager().ClearTimer(TimerHandle_BossSecondStopAttackTimer);
}

void AProjectArcadeCharacter::BossSecondSwapAttackBool()
{
  bossSecondCanBasicAttack = !bossSecondCanBasicAttack;
}

void AProjectArcadeCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (OverlappedComp == hitBox)
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
      FDamageEvent dmgEvt = FDamageEvent(*damageType);
      OtherActor->TakeDamage(NormalAttackDamage, dmgEvt, GetInstigatorController(), this);
    }

  if (OverlappedComp == tailHitBox)
    if (OtherActor && (OtherActor != this) && OtherComp &&
      OtherComp->GetCollisionObjectType() != ECollisionChannel::ECC_GameTraceChannel2 &&
      OtherComp->GetCollisionObjectType() != ECollisionChannel::ECC_GameTraceChannel3)
    {
      FDamageEvent dmgEvt = FDamageEvent(*damageType);
      OtherActor->TakeDamage(TwistAttackDamage, dmgEvt, GetInstigatorController(), this);
    }

  if (OverlappedComp == smashHitBox)
    if (OtherActor && (OtherActor != this) && OtherComp &&
      OtherComp->GetCollisionObjectType() != ECollisionChannel::ECC_GameTraceChannel2 &&
      OtherComp->GetCollisionObjectType() != ECollisionChannel::ECC_GameTraceChannel3)
    {
      FDamageEvent dmgEvt = FDamageEvent(*damageType);
      OtherActor->TakeDamage(SmashAttackDamage, dmgEvt, GetInstigatorController(), this);
    }

  if (OtherActor && (OtherActor != this) && OtherComp &&
    GetCapsuleComponent()->GetCollisionObjectType() == ECollisionChannel::ECC_GameTraceChannel4 &&
    OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_GameTraceChannel5)
  {
    UE_LOG(LogTemp, Warning, TEXT("AI Collided with SPAWNER"));

    ASpawner* tempSpawner = Cast<ASpawner>(OtherActor);
    if (tempSpawner != nullptr && spawner == nullptr)
    {
      UE_LOG(LogTemp, Warning, TEXT("Assinged SPAWNER"));
      spawner = tempSpawner;
    }
  }

  if (OverlappedComp == bossTailHitBox)
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
      FDamageEvent dmgEvt = FDamageEvent(*damageType);
      OtherActor->TakeDamage(BossTwistAttackDamage, dmgEvt, GetInstigatorController(), this);

    }

  if (OverlappedComp == bossSmashHitBox)
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
      FDamageEvent dmgEvt = FDamageEvent(*damageType);
      OtherActor->TakeDamage(BossSmashAttackDamage, dmgEvt, GetInstigatorController(), this);
    }

}

void AProjectArcadeCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
