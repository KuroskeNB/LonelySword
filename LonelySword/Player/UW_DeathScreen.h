// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_DeathScreen.generated.h"

/**
 * 
 */
UCLASS()
class LONELYSWORD_API UUW_DeathScreen : public UUserWidget
{
	GENERATED_BODY()
	public:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidgetOptional))
	class UButton* RestartGameButton;
	UPROPERTY(meta = (BindWidgetOptional))
    class UComboBoxString* ComboBox;
	UPROPERTY(meta = (BindWidgetOptional))
	class UButton* StartGameButton;
	UPROPERTY(meta = (BindWidgetOptional))
	class UButton* ExitButton;

	UFUNCTION()
	void OnRestartGameClicked();
	UFUNCTION()
	void OnStartGameClicked();
	UFUNCTION()
	void OnExitClicked();

	UFUNCTION()
    void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType); 
};
