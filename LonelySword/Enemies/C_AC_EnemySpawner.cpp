// Fill out your copyright notice in the Description page of Project Settings.


#include "C_AC_EnemySpawner.h"
#include "NavigationSystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Tools/C_Struct_WaveData.h"
#include "../Tools/C_GS_ArenaWaves.h"
#include "C_Interface_EnemyCharacter.h"

// Sets default values
AC_AC_EnemySpawner::AC_AC_EnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AC_AC_EnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	//SpawnThemAll();
}

void AC_AC_EnemySpawner::SpawnThemAll()
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem) return;
	UE_LOG(LogTemp, Display, TEXT("spawnthemall"));
	for (int32 i = 0; i < EnemyCount; ++i)
    {
		FActorSpawnParameters SpawnParameters;
					SpawnParameters.bDeferConstruction=EnemiesController!=nullptr;
					SpawnParameters.ObjectFlags|=RF_Transient;
		 FNavLocation RandomLocation;
		 NavSystem->GetRandomReachablePointInRadius(GetActorLocation(),SpawnRadius,RandomLocation);
		 //Spawning
		 RandomLocation.Location.Z+=100;
				APawn* SpawnedCharacter=GetWorld()->SpawnActor<APawn>(EnemiesClass,RandomLocation.Location,FRotator(),SpawnParameters);
				SpawnedCharacter->FinishSpawning(FTransform(FRotator(),RandomLocation,GetActorScale3D()));
				SpawnedCharacter->AIControllerClass=EnemiesController;
				SpawnedCharacter->SpawnDefaultController();
				DrawDebugSphere(GetWorld(), RandomLocation.Location, 50.0f, 12, FColor::Red, false, 30);
	}
}

void AC_AC_EnemySpawner::SpawnThem(FWaveDataArray SpawnData,int level)
{
  UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem) return;
	UE_LOG(LogTemp, Display, TEXT("spawnthemall"));
	for(int32 k = 0;k<SpawnData.WaveData.Num();++k)
	{
	for (int32 i = 0; i < SpawnData.WaveData[k].Count; ++i)
    {
		FActorSpawnParameters SpawnParameters;
					SpawnParameters.bDeferConstruction=SpawnData.WaveData[k].PawnController!=nullptr;
					SpawnParameters.ObjectFlags|=RF_Transient;
		 FNavLocation RandomLocation;
		 NavSystem->GetRandomReachablePointInRadius(FVector(-230,-510,150),SpawnRadius,RandomLocation);
		 //Spawning
		 RandomLocation.Location.Z+=100;
				APawn* SpawnedCharacter=GetWorld()->SpawnActor<APawn>(SpawnData.WaveData[k].PawnClass,RandomLocation.Location,FRotator(),SpawnParameters);
				SpawnedCharacter->FinishSpawning(FTransform(FRotator(),RandomLocation,GetActorScale3D()));
				SpawnedCharacter->AIControllerClass=SpawnData.WaveData[k].PawnController;
				SpawnedCharacter->SpawnDefaultController();

				if(SpawnedCharacter)
				{
                Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState())->MobSpawned();
				 if(SpawnedCharacter->GetClass()->ImplementsInterface(UC_Interface_EnemyCharacter::StaticClass()))
				 {
					IC_Interface_EnemyCharacter::Execute_SetLevelOfPower(SpawnedCharacter,level);
				 }
				}
	}
	}
}

// Called every frame
void AC_AC_EnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

