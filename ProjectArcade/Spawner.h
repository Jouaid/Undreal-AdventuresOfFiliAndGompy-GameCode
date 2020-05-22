// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectArcadeCharacter.h"
#include "Engine/TargetPoint.h"
#include "Spawner.generated.h"

UCLASS()
class PROJECTARCADE_API ASpawner : public AActor
{
  GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
    class USphereComponent* spwanSphere;
public:
  // Sets default values for this actor's properties
  ASpawner();
protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  TArray<TSubclassOf<AProjectArcadeCharacter>> objectsToSpawn;
  void Spawn();

public:
  UPROPERTY(EditAnywhere, Category = "Spawn")
    TArray<ATargetPoint*> placesToSpawn;
  UFUNCTION(BlueprintCallable)
    void AddToList(TSubclassOf<AProjectArcadeCharacter> objectToStore);
  UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
