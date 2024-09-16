// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_Struct_ResourceData.h"
#include "C_UW_ItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class LONELYSWORD_API UC_UW_ItemSlot : public UUserWidget
{
	GENERATED_BODY()
	public:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidgetOptional))
	class UButton* MainButton;
	UPROPERTY(meta = (BindWidgetOptional))
	class UTextBlock* CountText;
	UPROPERTY(EditAnywhere)
	float ItemCount=1;
	UFUNCTION()
	void ChangeItemCount(float NewCount);
	UFUNCTION()
	void UpdateSlotImage();
	UPROPERTY()
	FResourceData SlotData;
	UPROPERTY(meta = (BindWidgetOptional))
	class UButton* UseButton;
	UPROPERTY(meta = (BindWidgetOptional))
	class UButton* DropButton;
    UPROPERTY(meta = (BindWidgetOptional))
	class USizeBox* SizeBox1;
	UFUNCTION()
	void OnMainButtonClicked();
	UFUNCTION()
	void OnUseButtonClicked();
	UFUNCTION()
	void OnDropButtonClicked();

	bool bCanBeUsed=true;
	struct FTimerHandle Cooldown;
};
