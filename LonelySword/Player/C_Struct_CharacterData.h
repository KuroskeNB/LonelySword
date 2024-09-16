// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_Struct_CharacterData.generated.h"
/**
 * 
 */
class LONELYSWORD_API C_Struct_CharacterData
{
public:
	C_Struct_CharacterData();
	~C_Struct_CharacterData();
};

USTRUCT(BlueprintType)
struct LONELYSWORD_API FCharacterData
{
 GENERATED_USTRUCT_BODY()
 
 public:
    UPROPERTY(EditAnywhere)
	float MaxHP=100;
	UPROPERTY(VisibleAnywhere)
	float CurrentHP;
	UPROPERTY(EditAnywhere)
	float MaxEnergy=100;
	UPROPERTY(VisibleAnywhere)
	float CurrentEnergy;
	UPROPERTY(EditAnywhere)
	float EnergyMultiplier=1;
	UPROPERTY(EditAnywhere)
	float Damage=30;
	UPROPERTY(EditAnywhere)
	float Vampirism =0;
	UPROPERTY(EditAnywhere)
	float Stamina =0;
	UPROPERTY(EditAnywhere)
	float AttackSpeed =1;
	UPROPERTY(EditAnywhere)
	float AttackDistance =200;
};
