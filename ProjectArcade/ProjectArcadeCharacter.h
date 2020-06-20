// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "ProjectArcadeCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFirstAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShield);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShieldOff);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTwistAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSmashAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDash);

UCLASS(config = Game)
class AProjectArcadeCharacter : public ACharacter
{
  GENERATED_BODY()

    /** Camera boom positioning the camera behind the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

  /** Follow camera */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
    class USphereComponent* hitBox;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
    class USphereComponent* shieldBox;

  UPROPERTY(VisibleAnywhere, Category = "Comoponents")
    class UStaticMeshComponent* staticMeshShield;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
    class UCapsuleComponent* tailHitBox;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
    class USphereComponent* smashHitBox;


  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss", meta = (AllowPrivateAccess = "true"))
    class UCapsuleComponent* bossTailHitBox;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss", meta = (AllowPrivateAccess = "true"))
    class USphereComponent* bossSmashHitBox;

  UPROPERTY(EditAnywhere, Category = "Particle", meta = (AllowPrivateAccess = "true"))
    class UParticleSystem* smashParticle;

  UPROPERTY(EditAnywhere, Category = "Particle", meta = (AllowPrivateAccess = "true"))
    class UParticleSystem* twistParticle;

  UPROPERTY(EditAnywhere, Category = "Particle", meta = (AllowPrivateAccess = "true"))
    class UParticleSystem* dashParticle;
  
public:
  AProjectArcadeCharacter();


protected:
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  UPROPERTY(BlueprintReadOnly)
  AProjectArcadeCharacter* player2;
public:
  UPROPERTY(BlueprintReadOnly)
  int32 playerID = 0;

  UPROPERTY(BlueprintReadOnly)
    bool isPlayerOne;

  virtual void BeginPlay();

  // Called every frame
  virtual void Tick(float DeltaTime) override;

  /** Called for forwards/backward input */
  void MoveForward(float Value);

  /** Called for side to side input */
  void MoveRight(float Value);

  /** Returns CameraBoom subobject **/
  FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
  /** Returns FollowCamera subobject **/
  FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
  UPROPERTY(EditAnywhere, Category = "Camera Offset")
    FTransform offsetCamera;

  FTransform cameraOriginalPos;

  UFUNCTION(BlueprintImplementableEvent, Category = "Particle")
    void OnTwistAttackParticle(AActor* findMeInBlueprint); 
  UFUNCTION(BlueprintImplementableEvent, Category = "Particle")
    void OnSmashAttackParticle(AActor* findMeInBlueprint);
  UFUNCTION(BlueprintImplementableEvent, Category = "Particle")
    void OnDashParticle(AActor* findMeInBlueprint);

  UPROPERTY(EditAnywhere, Category = "Spawner")
    class ASpawner* spawner;

  UFUNCTION(BlueprintCallable)
    ASpawner* GetSpwaner();

  UPROPERTY(EditAnywhere, Category = "Health")
    class UHealthComponent* cHealth;

  UPROPERTY(EditAnywhere, Category = "Co-op")
    TSubclassOf<AProjectArcadeCharacter> player2Template;

  UFUNCTION()
    void AttackOne();
  UFUNCTION(BlueprintCallable)
    void AssignEventToFunction();
  UFUNCTION()
    void TestFunction();

  UPROPERTY(BlueprintAssignable, Category = "Test")
    FFirstAttack OnTestDelegate;

  /**Kills actor**/
  void Kill();

  UFUNCTION(BlueprintImplementableEvent)
  void Death(AActor* actor);

  bool dead;

  UFUNCTION(BlueprintCallable)
    bool IsDead();

  UFUNCTION()
    void OnHealthChanged(UHealthComponent* HealthComp, float currentHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

  UPROPERTY(BlueprintReadWrite, Category = Gameplay, Meta = (ExposeOnSpawn = true))
    TSubclassOf<UDamageType> damageType;

  //BASIC ATTACK LOGIC
  UFUNCTION(BlueprintCallable)
    void DoAttack();
  void StopAttack();
  FTimerHandle TimerHandle_AttackTimer;
  FTimerHandle TimerHandle_StopAttackTimer;
  bool canBasicAttack;
  void SwapAttackBool();
  UPROPERTY(EditAnywhere, Category = "Cooldown")
    float basicAttackCooldown;

  //Shield LOGIC
  void CallShieldOnInput();
  UFUNCTION()
    void DoShield();
  UFUNCTION()
    void StopShield();
  UFUNCTION(BlueprintCallable)
    void AssignShiledToFunction();
  UPROPERTY(BlueprintAssignable, Category = "Shield")
    FShield OnShieldDelegate;
  FTimerHandle TimerHandle_ShieldTimer;
  FTimerHandle TimerHandle_StopShieldTimer;
  UPROPERTY(EditAnywhere, Category = "Cooldown")
    float shieldCooldown;
  bool canShield;
  void SwapShieldBool();

  //Ground Smash Logic
  void CallSmashAttackOnInput();
  UFUNCTION()
    void DoSmashAttack();
  UFUNCTION()
    void StopSmashAttack();
  UFUNCTION(BlueprintCallable)
    void AssignSmashAttackToFunction();
  UPROPERTY(BlueprintAssignable, Category = "Smash")
    FSmashAttack OnSmashAttackDelegate;
  FTimerHandle TimerHandle_SmashAttackTimer;
  FTimerHandle TimerHandle_StopSmashAttackTimer;
  UPROPERTY(EditAnywhere, Category = "Cooldown")
    float SmashAttackCooldown;
  bool canSmashAttack;
  void SwapSmashAttackdBool();

  //Player 2 assginment
  void MoveRight_P2(float Value);
  void MoveForward_P2(float Value);
  void Jump_P2();
  void StopJump_P2();

  //Player 2 Attacking
  void DoAttack_P2();
  void StopAttack_P2();
  void SwapAttackBool_P2();

  //Player2
  //Twist Attack Logic
  void CallTwistAttackOnInput();
  UFUNCTION()
    void DoTwistAttack();
  UFUNCTION()
    void StopTwistAttack();
  UFUNCTION(BlueprintCallable)
    void AssignTwistAttackToFunction();
  UPROPERTY(BlueprintAssignable, Category = "Twist")
    FTwistAttack OnTwistAttackDelegate;
  FTimerHandle TimerHandle_TwistAttackTimer;
  FTimerHandle TimerHandle_StopTwistAttackTimer;
  UPROPERTY(EditAnywhere, Category = "Cooldown")
    float twistAttackCooldown;
  bool canTwistAttack;
  void SwapTwistAttackdBool();

  //Player2
  //Dashing Logic
  void CallDashOnInput();
  UFUNCTION()
    void DoDash();
  UFUNCTION(BlueprintCallable)
    void AssignDashToFunction();
  UPROPERTY(BlueprintAssignable, Category = "Dash")
    FDash OnDashDelegate;
  FTimerHandle TimerHandle_DashTimer;
  UPROPERTY(EditAnywhere, Category = "Cooldown")
    float DashCooldown;
  bool canDash;
  void SwapDashBool();
  UPROPERTY(EditAnywhere, Category = "Dash")
  float dashAmount;

  UPROPERTY(EditAnywhere, Category = "Damage")
    float SmashAttackDamage;
  UPROPERTY(EditAnywhere, Category = "Damage")
    float TwistAttackDamage;
  UPROPERTY(EditAnywhere, Category = "Damage")
    float NormalAttackDamage;

  UFUNCTION(BlueprintCallable)
    void Heal(float amount);

//Boss Attacks
  //Attack1
  UFUNCTION(BlueprintCallable)
    void BossAttack();
  void BossStopAttack();
  FTimerHandle TimerHandle_BossAttackTimer;
  FTimerHandle TimerHandle_BossStopAttackTimer;
  bool bossCanBasicAttack;
  void BossSwapAttackBool();
  UPROPERTY(EditAnywhere, Category = "Boss Cooldown")
    float bossBasicAttackCooldown;
  
  //Attack2
  UFUNCTION(BlueprintCallable)
    void BossSecondAttack();
  void BossSecondStopAttack();
  FTimerHandle TimerHandle_BossSecondAttackTimer;
  FTimerHandle TimerHandle_BossSecondStopAttackTimer;
  bool bossSecondCanBasicAttack;
  void BossSecondSwapAttackBool();
  UPROPERTY(EditAnywhere, Category = "Boss Cooldown")
    float bossSecondBasicAttackCooldown;

  UPROPERTY(EditAnywhere, Category = "Boss Damage")
    float BossSmashAttackDamage;
  UPROPERTY(EditAnywhere, Category = "Boss Damage")
    float BossTwistAttackDamage;

  //Audio
  UFUNCTION(BlueprintImplementableEvent)
    void GettingHitAudio(AActor* findMeInBlueprint);
  UFUNCTION(BlueprintImplementableEvent)
    void BasicAttackAudio(AActor* findMeInBlueprint);
  UFUNCTION(BlueprintImplementableEvent)
    void ShieldAttackAudio(AActor* findMeInBlueprint);
  UFUNCTION(BlueprintImplementableEvent)
    void DashAttackAudio(AActor* findMeInBlueprint);
  UFUNCTION(BlueprintImplementableEvent)
    void TwistAttackAudio(AActor* findMeInBlueprint);
  UFUNCTION(BlueprintImplementableEvent)
    void SmashAttackAudio(AActor* findMeInBlueprint);
  UFUNCTION(BlueprintImplementableEvent)
    void BossTwistAttackAudio(AActor* findMeInBlueprint);
  UFUNCTION(BlueprintImplementableEvent)
    void BossSmashAttackAudio(AActor* findMeInBlueprint);

  UFUNCTION(BlueprintImplementableEvent)
    void ActivateSnow(AActor* findMeInBlueprint);
  UFUNCTION(BlueprintImplementableEvent)
    void DeActivateSnow(AActor* findMeInBlueprint);

  UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
  UFUNCTION()
    void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};