// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GI_LonelySword.generated.h"

/**
 * 
 */
UCLASS()
class LONELYSWORD_API UGI_LonelySword : public UGameInstance
{
	GENERATED_BODY()

public:

UPROPERTY()
TSubclassOf<class ASwordDagameTypes> SwordDamageType;
void SetSwordDamageType(FString TypeName);
UPROPERTY(EditAnywhere)
TSubclassOf<class ASwordDagameTypes> ImpulseClass;
UPROPERTY(EditAnywhere)
TSubclassOf<class ASwordDagameTypes> SplashClass;
UPROPERTY(EditAnywhere)
TSubclassOf<class ASwordDagameTypes> PoisonClass;
UPROPERTY(EditAnywhere)
TSubclassOf<class ASwordDagameTypes> FreezeClass;

UFUNCTION()
TSubclassOf<class ASwordDagameTypes> GetDamageType(){return SwordDamageType;};
};
