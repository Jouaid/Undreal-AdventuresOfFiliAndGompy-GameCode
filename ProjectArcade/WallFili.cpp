// Fill out your copyright notice in the Description page of Project Settings.


#include "WallFili.h"
#include "Components/BoxComponent.h"

// Sets default values
AWallFili::AWallFili()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  overlapColldier = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Collider"));
  overlapColldier->SetBoundsScale(5.0f);
  overlapColldier->SetCollisionResponseToAllChannels(ECR_Ignore);
  overlapColldier->SetCollisionResponseToChannel(ECC_GameTraceChannel8, ECR_Overlap);
  overlapColldier->OnComponentBeginOverlap.AddDynamic(this, &AWallFili::OnOverlapBegin);
  overlapColldier->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWallFili::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWallFili::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (OtherActor && (OtherActor != this) && OtherComp &&
    OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_GameTraceChannel8)
  Destroy();
}

