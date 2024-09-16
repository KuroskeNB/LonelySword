// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_MainHud.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "../Tools/C_GS_ArenaWaves.h"


void UUW_MainHud::NativeConstruct()
{
    Super::NativeConstruct();
    
    if(StartNewWave)
    {
        StartNewWave->OnPressed.AddDynamic(this,&UUW_MainHud::OnStartNewWaveClicked);
        HideStartNewWave();
    }
}

void UUW_MainHud::OnStartNewWaveClicked()
{
 if(Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState()))
 {
   Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState())->StartNewLevel();
   if(GetOwningPlayer() && GetOwningPlayer()->bShowMouseCursor)
   {
   GetOwningPlayer()->SetShowMouseCursor(false);
   FInputModeGameOnly GameOnly;
   GetOwningPlayer()->SetInputMode(GameOnly);
   }
   HideStartNewWave();
 }
}

void UUW_MainHud::ShowStartNewWave()
{
    if(StartNewWave)
    {
        StartNewWave->SetVisibility(ESlateVisibility::Visible);
    }
}

void UUW_MainHud::HideStartNewWave()
{
    if(StartNewWave)
    {
        StartNewWave->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UUW_MainHud::AddPlayerEnergy(float percent)
{
    if(EnergyBar)
    {
     EnergyBar->SetPercent(EnergyBar->GetPercent()+percent);
     if(EnergyBar->GetPercent()>=1.f)
     {
      EnergyBar->SetPercent(1);
     }
    }
}

void UUW_MainHud::AddPlayerHealth(float percent)
{
    if(HealthBar)
    {
     HealthBar->SetPercent(percent);
     if(HealthBar->GetPercent()>=1.f)
     {
      HealthBar->SetPercent(1);
     }
    }
}

void UUW_MainHud::DrainingUpdate(float Damage, float CurrentHP, float MaxHP)
{
    if(DamageAmount)
    {
        DamageAmount->SetText(FText::FromString(FString::Printf(TEXT("%.f"),Damage)));
    }
    if (CurrentHealth)
    {
        CurrentHealth->SetText(FText::FromString(FString::Printf(TEXT("%.f"),CurrentHP)));
    }
    if (MaxHealth)
    {
        MaxHealth->SetText(FText::FromString(FString::Printf(TEXT("%.f"),MaxHP)));
    }
    
}
