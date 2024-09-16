// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class LONELYSWORD_API UUW_MainMenu : public UUserWidget
{
	GENERATED_BODY()
	public:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidgetOptional))
	class UButton* SetupSwordButton;
	UPROPERTY(meta = (BindWidgetOptional))
    class UComboBoxString* ComboBox;
	UPROPERTY(meta = (BindWidgetOptional))
	class UButton* StartGameButton;
    
	UFUNCTION()
	void OnSetupSwordButtonClicked();
	UFUNCTION()
	void OnStartGameButtonClicked();

	UFUNCTION()
    void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType); 
};
