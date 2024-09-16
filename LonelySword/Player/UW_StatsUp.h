// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_StatsUp.generated.h"

/**
 * 
 */
UCLASS()
class LONELYSWORD_API UUW_StatsUp : public UUserWidget
{
	GENERATED_BODY()
	public:
		UPROPERTY(meta = (BindWidgetOptional))
	class UButton* Button;

    virtual void NativeConstruct() override;
	UFUNCTION()
	void LoadButton(enum EStatsToUp stat,class UTexture2D* texture, float lvl);
    enum EStatsToUp Stat;
	float Level;
	private:
	UFUNCTION()
	void OnButtonClicked();
};
