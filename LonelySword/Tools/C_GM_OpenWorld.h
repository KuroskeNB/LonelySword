// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "C_GM_OpenWorld.generated.h"

/**
 * 
 */
UCLASS()
class LONELYSWORD_API AC_GM_OpenWorld : public AGameMode
{
	GENERATED_BODY()

public:
virtual void BeginPlay() override;
UPROPERTY(EditAnywhere)
TSubclassOf<class AC_AC_EnemySpawner> EnemySpawnerClass;
};
