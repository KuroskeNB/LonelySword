// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "C_PC_PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class LONELYSWORD_API AC_PC_PlayerCharacter : public APlayerController
{
	GENERATED_BODY()
	public:
    
	UFUNCTION()
	void OpenInventory();
    bool bInventoryIsOpen=false;
    UFUNCTION()
	void OpenMenu();
	UFUNCTION()
	void AddPickedToInventory(AActor* Picked);
    
	UFUNCTION()
	void PawnHealthChanged(float change);
	UFUNCTION()
	void PawnEnergyChanged(float change);
	
	UFUNCTION()
	void PawnDrainingUpdate(float Damage,float CurrentHP,float MaxHP);
	UFUNCTION()
	void PlayerUsedSlot(int index);

	protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> InventoryClass;
	class UUserWidget* InventoryWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> MainHudClass;
	class UUserWidget* MainHudWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> MenuClass;
	class UUserWidget* MenuWidget;

	UFUNCTION()
	void OnFinishedWave(float level);
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> StatsUpClass;
	class UUserWidget* StatsUpWidget;

};
