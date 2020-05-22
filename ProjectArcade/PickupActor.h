// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupActor.generated.h"

class APowerupActor;

UCLASS()
class PROJECTARCADE_API APickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category= "Comoponents")
		class USphereComponent* sphereComp;
	UPROPERTY(VisibleAnywhere, Category = "Comoponents")
		class UStaticMeshComponent* staticMeshRock;

	UPROPERTY(EditDefaultsOnly, Category = "PickupActor")
	 TSubclassOf<APowerupActor> powerupClass;
	APowerupActor* powerupInstance;
	
	UPROPERTY(EditInstanceOnly, Category = "PickupActor")
	float coolDownDurtaion;

	FTimerHandle TimerHandle_RespawnTimer;


	void Respawn();

public:	

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
