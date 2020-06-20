// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ProjectArcadeCharacter.h"
#include "ArcadeCamera.generated.h"

UCLASS()
class PROJECTARCADE_API AArcadeCamera : public APawn
{
  GENERATED_BODY()

    /** Camera boom positioning the camera behind the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

  /** Follow camera */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;
public:
  // Sets default values for this pawn's properties
  AArcadeCamera();
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
    AProjectArcadeCharacter* playerOne;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
    AProjectArcadeCharacter* playerTwo;


  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offset")
    int32 xOffSet;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offset")
    int32 yOffSet;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offset")
    int32 zOffSet;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance")
    int32 distCheckNumber;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
  float floatDist;

  bool splitCamera;

  APlayerController* myCamera;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    AActor* actorToMoveTo;

  FVector dist;

  UFUNCTION(BlueprintImplementableEvent)
    void SplitCamera(AActor* findMeInBlueprint);
  UFUNCTION(BlueprintImplementableEvent)
    void JoinCamera(AActor* findMeInBlueprint);
protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  void MoveCamera();


public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintCallable)
    void SetPlayers(AProjectArcadeCharacter* getPlayerOne, AProjectArcadeCharacter* getPlayerTwo);

  /** Returns CameraBoom subobject **/
  FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
  /** Returns FollowCamera subobject **/
  FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
