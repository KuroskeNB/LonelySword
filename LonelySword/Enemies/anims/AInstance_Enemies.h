// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AInstance_Enemies.generated.h"

/**
 * 
 */
UCLASS()
class LONELYSWORD_API UAInstance_Enemies : public UAnimInstance
{
	GENERATED_BODY()

public:
UFUNCTION(BlueprintPure,BlueprintCallable, meta = (BlueprintThreadSafe))
bool IsEnemyAlive();
};
