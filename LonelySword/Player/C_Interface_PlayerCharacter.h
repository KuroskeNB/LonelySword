// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "C_Interface_PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerStatus : uint8    // (uint8, uint16)
{
	InBattle UMETA(DisplayName = "In Battle"),
	OutOfBattle UMETA(DisplayName = "Out Of Battle")
};
UENUM(BlueprintType)
enum class EPlayerActions : uint8    // (uint8, uint16)
{
	Attacking UMETA(DisplayName = "Attacking"),
	Nothing UMETA(DisplayName = "Nothing")
};

UENUM(BlueprintType)
enum class EStatsToUp : uint8    // (uint8, uint16)
{
	AttackSpeed UMETA(DisplayName = "AttackSpeed"),
	AttackStrength UMETA(DisplayName = "AttackStrength"),
	Vampirism UMETA(DisplayName = "Vampirism"),
	Stamina UMETA(DisplayName = "Stamina"),
	WalkSpeed UMETA(DisplayName = "WalkSpeed")
};

UENUM(BlueprintType)
enum class EPotionTypes : uint8 
{
	HealthPotion UMETA(DisplayName = "HealthPotion"),
	DamagePotion UMETA(DisplayName = "DamagePotion"),
	EnergyPotion UMETA(DisplayName = "EnergyPotion"),
	VampiricPotion UMETA(DisplayName = "VampiricPotion")
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UC_Interface_PlayerCharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LONELYSWORD_API IC_Interface_PlayerCharacter
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

UFUNCTION(BlueprintNativeEvent)
void JoinFight();
UFUNCTION(BlueprintNativeEvent)
void LeaveFight();
UFUNCTION(BlueprintNativeEvent)
void UpStat(EStatsToUp stat, float level);
UFUNCTION(BlueprintNativeEvent)
EPlayerStatus GetPlayerStatus();
UFUNCTION(BlueprintNativeEvent)
AActor* GetDamageType();
UFUNCTION(BlueprintNativeEvent)
void DrinkPotion(EPotionTypes type);
};
