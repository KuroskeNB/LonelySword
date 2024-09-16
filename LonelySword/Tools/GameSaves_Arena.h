// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameSaves_Arena.generated.h"

/**
 * 
 */
UCLASS()
class LONELYSWORD_API UGameSaves_Arena : public USaveGame
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditAnywhere)
	int MaximalLevel;
};
