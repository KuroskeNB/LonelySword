// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "C_AI_Player.generated.h"

/**
 * 
 */
UCLASS()
class LONELYSWORD_API UC_AI_Player : public UAnimInstance
{
	GENERATED_BODY()
public:

UFUNCTION(BlueprintPure,BlueprintCallable, meta = (BlueprintThreadSafe))
bool IsPlayerFighting();
};
