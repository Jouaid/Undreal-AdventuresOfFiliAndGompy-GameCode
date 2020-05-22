// Fill out your copyright notice in the Description page of Project Settings.


#include "WallGompy.h"
#include "Components/BoxComponent.h"

// Sets default values
AWallGompy::AWallGompy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  overlapColldier = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Collider"));
  overlapColldier->SetBoundsScale(5.0f);
  overlapColldier->SetCollisionResponseToAllChannels(ECR_Ignore);
  overlapColldier->SetCollisionResponseToChannel(ECC_GameTraceChannel7, ECR_Overlap);
  overlapColldier->OnComponentBeginOverlap.AddDynamic(this, &AWallGompy::OnOverlapBegin);
  overlapColldier->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AWallGompy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWallGompy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  Destroy();
}

