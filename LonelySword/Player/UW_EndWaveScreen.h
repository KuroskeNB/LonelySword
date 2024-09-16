// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_Interface_PlayerCharacter.h"
#include "UW_EndWaveScreen.generated.h"

/**
 * 
 */
UCLASS()
class LONELYSWORD_API UUW_EndWaveScreen : public UUserWidget
{
	GENERATED_BODY()
	public:
    
	UPROPERTY(EditAnywhere)
	TMap<EStatsToUp,class UTexture2D*>StatTypes;
    
	UPROPERTY()
	TArray<float> arrayStats;
	UFUNCTION()
	uint8 GetStatId();
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnStatChoosed();

	private:

	UPROPERTY(meta = (BindWidgetOptional))
	class UUW_StatsUp* FirstStat;
	UPROPERTY(meta = (BindWidgetOptional))
	class UUW_StatsUp* SecondStat;
	UPROPERTY(meta = (BindWidgetOptional))
	class UUW_StatsUp* ThirdStat;
};
