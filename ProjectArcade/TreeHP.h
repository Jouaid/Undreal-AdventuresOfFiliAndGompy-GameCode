// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupActor.h"
#include "TreeHP.generated.h"

UCLASS()
class PROJECTARCADE_API ATreeHP : public AActor
{
  GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comops", meta = (AllowPrivateAccess = "true"))
    class UCapsuleComponent* blockCollider;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comops", meta = (AllowPrivateAccess = "true"))
    class UCapsuleComponent* overlapColldier;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comops", meta = (AllowPrivateAccess = "true"))
    class UCapsuleComponent* spawnColldier;

public:
  // Sets default values for this actor's properties
  ATreeHP();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  int32 objectCount;

  UPROPERTY(EditDefaultsOnly, Category = "PickupActor")
  	TSubclassOf<APickupActor> powerupClass;

public:
  UPROPERTY(EditAnywhere, Category = "Spawn")
    int32 howMany;

  UPROPERTY(EditAnywhere, Category = "Spawn")
    int32 distance;

  UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
