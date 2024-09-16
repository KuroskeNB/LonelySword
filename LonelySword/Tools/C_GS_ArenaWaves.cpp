#include "C_GS_ArenaWaves.h"
#include "../Enemies/C_AC_EnemySpawner.h"
#include "C_Struct_WaveData.h"
#include "Kismet/GameplayStatics.h"

void AC_GS_ArenaWaves::BeginPlay()
{
    Super::BeginPlay();
    //StartNewLevel();
}

void AC_GS_ArenaWaves::StartNewLevel()
{
  TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AC_AC_EnemySpawner::StaticClass(), FoundActors);
  if(FoundActors.Num() > 0)
  {
    auto WavesData = LevelsData->GetRowNames();
    int lvl = 0;
    for(auto Row : WavesData)
    {
        if(lvl==CurrentLevel)
        {
        Cast<AC_AC_EnemySpawner>(FoundActors[0])->SpawnThem(*LevelsData->FindRow<FWaveDataArray>(Row,""),CurrentLevel);
        }
        lvl++;
    }
  }
}

void AC_GS_ArenaWaves::FinishedWave(float level)
{
  OnFinishedWave.Broadcast(level);
}