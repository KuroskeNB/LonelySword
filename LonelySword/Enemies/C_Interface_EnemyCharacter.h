// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "C_Interface_EnemyCharacter.generated.h"


UENUM(BlueprintType)
enum class EEnemyStatus : uint8    // (uint8, uint16)
{
	InBattle UMETA(DisplayName = "In Battle"),
	OutOfBattle UMETA(DisplayName = "Out Of Battle")
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UC_Interface_EnemyCharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LONELYSWORD_API IC_Interface_EnemyCharacter
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
UFUNCTION(BlueprintNativeEvent)
void EnemyPlayMontage(class UAnimMontage* MyAnimMontage);
UFUNCTION(BlueprintNativeEvent)
void EnemyAttack();
UFUNCTION(BlueprintNativeEvent)
void CheckDamageForEffects(AActor* DamageCauser);
UFUNCTION(BlueprintNativeEvent)
void DropItems();
UFUNCTION(BlueprintNativeEvent)
bool IsAlive();
UFUNCTION(BlueprintNativeEvent)
float GetPawnAttackDistance();
UFUNCTION(BlueprintNativeEvent)
AActor* GetDamageType();
UFUNCTION(BlueprintNativeEvent)
void BossUltimate(AActor* Actor);
UFUNCTION(BlueprintNativeEvent)
bool IsAbleToMove();
UFUNCTION(BlueprintNativeEvent)
void SetLevelOfPower(int level);
};
