// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_AC_EnemySpawner.generated.h"

UCLASS()
class LONELYSWORD_API AC_AC_EnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_AC_EnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
	void SpawnThemAll();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    void SpawnThem(struct FWaveDataArray SpawnData,int level);

UPROPERTY(EditAnywhere)
TSubclassOf<APawn> EnemiesClass;
UPROPERTY(EditAnywhere)
float EnemyCount=5;
UPROPERTY(EditAnywhere)
float SpawnRadius =600;
UPROPERTY(EditAnywhere)
TSubclassOf<class AController> EnemiesController;
};
