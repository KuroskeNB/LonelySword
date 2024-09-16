// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PC_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class LONELYSWORD_API APC_MainMenu : public APlayerController
{
	GENERATED_BODY()
	protected:

	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> MainMenuClass;
	class UUserWidget* MainMenuWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> PreStartClass;
	class UUserWidget* PreStartWidget;
};
