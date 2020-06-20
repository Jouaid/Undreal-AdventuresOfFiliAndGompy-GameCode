// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeHP.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"

// Sets default values
ATreeHP::ATreeHP()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  blockCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
  blockCollider->SetCapsuleSize(25, 12);
  blockCollider->SetCollisionResponseToAllChannels(ECR_Block);
  RootComponent = blockCollider;

  overlapColldier = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Overlap Collider"));
  overlapColldier->SetCapsuleSize(25, 12);
  overlapColldier->SetCollisionResponseToAllChannels(ECR_Ignore);
  overlapColldier->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Overlap);
  overlapColldier->OnComponentBeginOverlap.AddDynamic(this, &ATreeHP::OnOverlapBegin);
  overlapColldier->SetupAttachment(RootComponent);

  spawnColldier = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Spawn Collider"));
  spawnColldier->SetCapsuleSize(25, 12);
  spawnColldier->SetCollisionResponseToAllChannels(ECR_Ignore);

}

// Called when the game starts or when spawned
void ATreeHP::BeginPlay()
{
  Super::BeginPlay();
}

void ATreeHP::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (howMany > 0)
    if (OtherActor && (OtherActor != this) && OtherComp &&
      OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_GameTraceChannel6)
    {
      if (powerupClass == nullptr)
      {
        UE_LOG(LogTemp, Warning, TEXT("PowerupClass is nullptr in %s. CHECK THE BLUEPRINT"), *GetName());
        return;
      }
      FTransform trans;
      trans = spawnColldier->GetComponentTransform();
      FActorSpawnParameters spawnParams;
      spawnParams.Owner = this;
      GetWorld()->SpawnActor<APickupActor>(powerupClass, trans, spawnParams);
      howMany--;
    }
}



