// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "ProjectArcadeCharacter.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
  // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
  // off to improve performance if you don't need them.
  PrimaryComponentTick.bCanEverTick = false;
  maxHealth = 100;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
  Super::BeginPlay();
  currentHealth = maxHealth;
  Owner = GetOwner();
  if (Owner && !overrideTakeDamage)
  {
    Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);

    player = Cast<AProjectArcadeCharacter>(Owner);
  }
}

void UHealthComponent::Reset()
{
  currentHealth = maxHealth;
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
  if (Damage <= 0)
    return;

  currentHealth = FMath::Clamp(currentHealth - Damage, 0.0f, maxHealth);
  UE_LOG(LogTemp, Warning, TEXT("HEALTH =  %s"), *FString::SanitizeFloat(currentHealth));

  if (currentHealth <= 0)
    Cast<AProjectArcadeCharacter>(Owner)->Kill();
  OnHealthChanged.Broadcast(this, currentHealth, Damage, DamageType, InstigatedBy, DamageCauser);

  if (player->playerID == 1)
    OnHealthChangedPlayerOne.Broadcast(this, currentHealth, Damage, DamageType, InstigatedBy, DamageCauser);
  else if (player->playerID == 2)
    OnHealthChangedPlayerTwo.Broadcast(this, currentHealth, Damage, DamageType, InstigatedBy, DamageCauser);
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::Heal(float HealAmount)
{
  if (HealAmount <= 0.0f || currentHealth <= 0.0f)
    return;

  currentHealth = FMath::Clamp(currentHealth + HealAmount, 0.0f, maxHealth);
  UE_LOG(LogTemp, Warning, TEXT("HEALTH Change=  %s (+%s)"), *FString::SanitizeFloat(currentHealth), *FString::SanitizeFloat(HealAmount));
  OnHealthChanged.Broadcast(this, currentHealth, -HealAmount, nullptr, nullptr, nullptr);

  if (player->playerID == 1)
    OnHealthChangedPlayerOne.Broadcast(this, currentHealth, -HealAmount, nullptr, nullptr, nullptr);
  else if (player->playerID == 2)
    OnHealthChangedPlayerTwo.Broadcast(this, currentHealth, -HealAmount, nullptr, nullptr, nullptr);
}

