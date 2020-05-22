// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor.h"
#include "Components/SphereComponent.h"
#include "PowerupActor.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APickupActor::APickupActor()
{
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	sphereComp->SetSphereRadius(75.0f);
	RootComponent = sphereComp;

	staticMeshRock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("staticMesh"));
	staticMeshRock->SetRelativeRotation(FRotator(90, 0.0f, 0.0f));
	staticMeshRock->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
	Super::BeginPlay();

	Respawn();
}

void APickupActor::Respawn()
{
	if (powerupClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PowerupClass is nullptr in %s. CHECK THE BLUEPRINT"), *GetName());
		return;
	}

	FActorSpawnParameters spawnParams;

	powerupInstance = GetWorld()->SpawnActor<APowerupActor>(powerupClass, GetTransform(), spawnParams);
}

void APickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//TODO: Grant a power up to player
	if (powerupInstance)
	{
		powerupInstance->ActivatePowerup(OtherActor);
		powerupInstance = nullptr;

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer,this,&APickupActor::Respawn,coolDownDurtaion);
	}
}



