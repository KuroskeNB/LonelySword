// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "C_Interface_PlayerCharacter.h"
#include "C_Struct_ResourceData.generated.h"

/**
 * 
 */
class LONELYSWORD_API C_Struct_ResourceData
{
public:
	C_Struct_ResourceData();
	~C_Struct_ResourceData();
};

USTRUCT(BlueprintType)
struct LONELYSWORD_API FResourceData : public FTableRowBase
{
 GENERATED_USTRUCT_BODY()
 
 public:
    UPROPERTY(EditAnywhere)
	class UTexture2D* Icon;
	UPROPERTY(EditAnywhere)
	FName ResourceName;
	UPROPERTY(EditAnywhere)
	EPotionTypes type;
	UPROPERTY(EditAnywhere)
	float CooldownTime=1;
};
