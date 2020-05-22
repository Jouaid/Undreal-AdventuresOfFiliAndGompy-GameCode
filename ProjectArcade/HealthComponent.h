// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

//On HEALTH CHANGED DELEGATE
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UHealthComponent*, HealthComp, float, currentHealth, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTARCADE_API UHealthComponent : public UActorComponent
{
  GENERATED_BODY()

public:
  // Sets default values for this component's properties
  UHealthComponent();

  UFUNCTION()
    void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

  void Reset();

  UPROPERTY(EditAnywhere, Category = "Health")
    bool overrideTakeDamage;

  UPROPERTY(BlueprintReadOnly, Category = "Health")
    float currentHealth;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
    float maxHealth;

protected:
  // Called when the game starts
  virtual void BeginPlay() override;

  AActor* Owner;
public:
  // Called every frame
  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
  

 class AProjectArcadeCharacter* player;

  UFUNCTION()
  void Heal(float HealAmount);

  UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnHealthChangedSignature OnHealthChanged;
  UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnHealthChangedSignature OnHealthChangedPlayerOne;
  UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnHealthChangedSignature OnHealthChangedPlayerTwo;
};
