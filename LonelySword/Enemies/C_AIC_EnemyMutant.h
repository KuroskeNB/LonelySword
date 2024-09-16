// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "C_AIC_EnemyMutant.generated.h"

/**
 * 
 */
UCLASS()
class LONELYSWORD_API AC_AIC_EnemyMutant : public AAIController
{
	GENERATED_BODY()
	public:
	AC_AIC_EnemyMutant();
    virtual void BeginPlay() override;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnSight(AActor* Actor, FAIStimulus Stimulus);
	
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehaviorTree;

	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionComponent* AIPerceptions;

    
	class UAISenseConfig_Prediction* PredictionConfig;
	class UAISenseConfig_Sight* SightConfig;
 

 private:
 UPROPERTY()
 FVector StartLocation;

 UPROPERTY()
 float AttackDistance=0;
};
