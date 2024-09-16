// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "C_Struct_WaveData.generated.h"

/**
 * 
 */
class LONELYSWORD_API C_Struct_WaveData
{
public:
	C_Struct_WaveData();
	~C_Struct_WaveData();
};

USTRUCT(BlueprintType)
struct LONELYSWORD_API FWaveData : public FTableRowBase
{
 GENERATED_USTRUCT_BODY()
 
 public:
    UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> PawnClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AController> PawnController;
	UPROPERTY(EditAnywhere)
	float Count=1;
	UPROPERTY(EditAnywhere)
	float PowerLevel=1;
};

USTRUCT(BlueprintType)
struct LONELYSWORD_API FWaveDataArray : public FTableRowBase
{
 GENERATED_USTRUCT_BODY()
 
 public:
    UPROPERTY(EditAnywhere)
	TArray<FWaveData> WaveData;
};