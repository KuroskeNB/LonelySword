#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "C_GS_ArenaWaves.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FWaveEndEvent, float);

UCLASS()
class LONELYSWORD_API AC_GS_ArenaWaves : public AGameState
{
	GENERATED_BODY()
	public:

	FWaveEndEvent OnFinishedWave;

	UPROPERTY(EditAnywhere)
	float CurrentLevel=0;
    float MobsCount=0;
	UFUNCTION()
    float GetCurrentLevel(){return CurrentLevel;};
	virtual void BeginPlay() override;
	void StartNewLevel();
 
    void MobDied() {MobsCount-=1; if(MobsCount==0) {CurrentLevel++;FinishedWave(CurrentLevel); UE_LOG(LogTemp, Display, TEXT("newave"));}};
	void MobSpawned() {MobsCount+=1;};
	void FinishedWave(float level);
	UPROPERTY(EditAnywhere)
	class UDataTable* LevelsData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class AC_AC_EnemySpawner> SpawnerObj; 
};
