// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_MainHud.generated.h"

/**
 * 
 */
UCLASS()
class LONELYSWORD_API UUW_MainHud : public UUserWidget
{
	GENERATED_BODY()
	public:

	UPROPERTY(meta = (BindWidgetOptional))
	class UButton* StartNewWave;
	UPROPERTY(meta = (BindWidgetOptional))
	class UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidgetOptional))
	class UProgressBar* EnergyBar;
	UPROPERTY(meta = (BindWidgetOptional))
	class UTextBlock* DamageAmount;
	UPROPERTY(meta = (BindWidgetOptional))
	class UTextBlock* CurrentHealth;
	UPROPERTY(meta = (BindWidgetOptional))
	class UTextBlock* MaxHealth;
	UFUNCTION()
	void ShowStartNewWave();
	UFUNCTION()
	void HideStartNewWave();

    UFUNCTION()
	void AddPlayerEnergy(float percent);
	UFUNCTION()
	void AddPlayerHealth(float percent);
    UFUNCTION()
	void DrainingUpdate(float Damage,float CurrentHP,float MaxHP);
	

	private:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnStartNewWaveClicked();
   
};
