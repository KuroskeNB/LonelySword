// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PC_PlayerCharacter.h"
#include "C_UW_PlayerInventory.h"
#include "C_AC_PickableResource.h"
#include "UW_StatsUp.h"
#include "UW_MainHud.h"
#include "../Tools/C_GS_ArenaWaves.h"
#include "../Tools/GameSaves_Arena.h"
#include "Kismet/GameplayStatics.h"

void AC_PC_PlayerCharacter::OpenInventory()
{
    if(InventoryWidget && !bInventoryIsOpen)
    {
        UE_LOG(LogTemp, Display, TEXT("open"));
      InventoryWidget->SetVisibility(ESlateVisibility::Visible);
     FInputModeGameAndUI GameAndUI;
        SetInputMode(GameAndUI);
     SetShowMouseCursor(true);
    bInventoryIsOpen=true;
    }
    else if(InventoryWidget && bInventoryIsOpen)
    {
         UE_LOG(LogTemp, Display, TEXT("close"));
        InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
       FInputModeGameOnly GameOnly;
     SetInputMode(GameOnly);
     SetShowMouseCursor(false);
    bInventoryIsOpen=false;
    }
}

void AC_PC_PlayerCharacter::AddPickedToInventory(AActor *Picked)
{
    if(!Picked) return;
    AC_AC_PickableResource* PickedResource = Cast<AC_AC_PickableResource>(Picked);
    if(InventoryWidget && PickedResource && !PickedResource->Resources.IsEmpty())
    {
        for(auto pair : PickedResource->Resources)
        {
            UE_LOG(LogTemp, Display, TEXT("for pair"));
            int Count = FMath::RandRange(0,(int)pair.Value);
            if(Cast<UC_UW_PlayerInventory>(InventoryWidget)&& Count!=0)
        Cast<UC_UW_PlayerInventory>(InventoryWidget)->AddToInventory(pair.Key,Count);
        }
      PickedResource->Destroy();
    }
}

void AC_PC_PlayerCharacter::PawnHealthChanged(float change)
{
    if(MainHudWidget)
    {
        Cast<UUW_MainHud>(MainHudWidget)->AddPlayerHealth(change);
    }
}

void AC_PC_PlayerCharacter::PawnEnergyChanged(float change)
{
    if(MainHudWidget)
    Cast<UUW_MainHud>(MainHudWidget)->AddPlayerEnergy(change);
}

void AC_PC_PlayerCharacter::PawnDrainingUpdate(float Damage, float CurrentHP, float MaxHP)
{
    if(MainHudWidget)
    Cast<UUW_MainHud>(MainHudWidget)->DrainingUpdate(Damage,CurrentHP,MaxHP);
}

void AC_PC_PlayerCharacter::PlayerUsedSlot(int index)
{
    if(InventoryWidget)
    {
        UE_LOG(LogTemp, Display, TEXT("pc index %f"),index);
        Cast<UC_UW_PlayerInventory>(InventoryWidget)->UseSlot(index);
    }
}

void AC_PC_PlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    if(InventoryClass && IsLocalController())
    {
        InventoryWidget=CreateWidget(this,InventoryClass);
        if (InventoryWidget)
        {
            InventoryWidget->AddToViewport();
            //InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }
    if(MainHudClass && IsLocalController())
    {
        MainHudWidget=CreateWidget(this,MainHudClass);
        if (MainHudWidget)
        {
            MainHudWidget->AddToViewport();
        }
    }
    if(Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState()))
    {
        Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState())->OnFinishedWave.AddUObject(this,&AC_PC_PlayerCharacter::OnFinishedWave);
    }
    FInputModeGameOnly GameOnly;
    SetInputMode(GameOnly);
    SetShowMouseCursor(false);
    OnFinishedWave(0);
}

void AC_PC_PlayerCharacter::OpenMenu()
{
    if(MenuClass && IsLocalController())
    MenuWidget=CreateWidget(this,MenuClass);
       if (MenuWidget)
        {
            MenuWidget->AddToViewport();
            FInputModeUIOnly UIOnly;
        SetInputMode(UIOnly);
        SetShowMouseCursor(true);
        }

}

void AC_PC_PlayerCharacter::OnFinishedWave(float level)
{
    UE_LOG(LogTemp, Display, TEXT("finished level: %f"),level);
    if(StatsUpClass)
    {
        UE_LOG(LogTemp, Display, TEXT("create stats up"));
        StatsUpWidget=CreateWidget(this,StatsUpClass);
        if(!StatsUpWidget) return;
                UE_LOG(LogTemp, Display, TEXT("stats up"));
        StatsUpWidget->AddToViewport();
        FInputModeGameAndUI UIOnly;
        SetInputMode(UIOnly);
        SetShowMouseCursor(true);
    }
    if(MainHudWidget)
    {
        Cast<UUW_MainHud>(MainHudWidget)->ShowStartNewWave();
    }
    if(Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState()))
	{
		UGameSaves_Arena* SaveGameInstance = Cast<UGameSaves_Arena>(UGameplayStatics::CreateSaveGameObject(UGameSaves_Arena::StaticClass()));
		if(SaveGameInstance)
		{
        SaveGameInstance->MaximalLevel=Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState())->GetCurrentLevel();
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, "SaveSlot", 0);
		}
	}
}


