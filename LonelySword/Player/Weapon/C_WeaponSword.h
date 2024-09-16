// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_WeaponSword.generated.h"

UCLASS()
class LONELYSWORD_API AC_WeaponSword : public AActor
{
	GENERATED_BODY()

private:
  UPROPERTY(EditAnywhere)
  TObjectPtr<USkeletalMeshComponent> SwordMesh;
  
  UFUNCTION()
  void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	// Sets default values for this actor's properties
	AC_WeaponSword();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
