// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_UW_PlayerInventory.generated.h"

/**
 * 
 */
UCLASS()
class LONELYSWORD_API UC_UW_PlayerInventory : public UUserWidget
{
	GENERATED_BODY()
	public:

	UPROPERTY(meta = (BindWidgetOptional))
    class UWrapBox* MainWrapBox;
	virtual void NativeConstruct() override;
    
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UDataTable> ResourcesDataTable;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ItemSlotClass;
	UFUNCTION()
	void AddToInventory(FName ItemName,float ItemCount);
	UFUNCTION()
	void AddSlot(FName ItemName,float ItemCount);
	UFUNCTION()
	void UseSlot(int index);
};
