// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"

// Sets default values
ASpawner::ASpawner()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
  spwanSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Spawn Sphere"));
  spwanSphere->SetSphereRadius(200);
  spwanSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
  spwanSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
  spwanSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
  spwanSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Overlap);
  spwanSphere->OnComponentBeginOverlap.AddDynamic(this, &ASpawner::OnOverlapBegin);
  spwanSphere->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
  Super::BeginPlay();
}

void ASpawner::Spawn()
{
  UWorld* world = GetWorld();

  if (placesToSpawn.Num() > 0)
    for (size_t i = 0; i < objectsToSpawn.Num(); i++)
    {
      UE_LOG(LogTemp, Warning, TEXT("SPAWNED"));
      int32 location = rand() % placesToSpawn.Num();
      AProjectArcadeCharacter* player = world->SpawnActor<AProjectArcadeCharacter>(objectsToSpawn[i],
                                                                                   placesToSpawn[location]->GetTransform().GetLocation(),
                                                                                   placesToSpawn[location]->GetTransform().GetRotation().Rotator());
    }
  objectsToSpawn.Empty();
}

void ASpawner::AddToList(TSubclassOf<AProjectArcadeCharacter> objectToStore)
{
  UE_LOG(LogTemp, Warning, TEXT("ADD TO LIST CALLED"));
  objectsToSpawn.Add(objectToStore);
}

void ASpawner::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

  if (OtherActor && (OtherActor != this) && OtherComp && OtherComp->GetCollisionObjectType() != ECollisionChannel::ECC_GameTraceChannel4 /*check if its AI*/)
  {
    UE_LOG(LogTemp, Warning, TEXT("COLLISION"));
    if (objectsToSpawn.Num() > 0)
    {
      UE_LOG(LogTemp, Warning, TEXT("WE HAVE OBJECTS TO SPAWN: %d"), objectsToSpawn.Num());
      Spawn();
    }
  }
}