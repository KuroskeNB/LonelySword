// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_EnemyHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class LONELYSWORD_API UUW_EnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()
	public:
    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetHealthPercent(float Percent);

protected:
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HealthProgressBar;
};
