// Fill out your copyright notice in the Description page of Project Settings.


#include "C_GM_OpenWorld.h"

void AC_GM_OpenWorld::BeginPlay()
{
    Super::BeginPlay();
    if(EnemySpawnerClass)
    {
        AActor* spawner=GetWorld()->SpawnActor<AActor>(EnemySpawnerClass);
        spawner->SetActorLocation(FVector(-230,-510,120));
    }
}
