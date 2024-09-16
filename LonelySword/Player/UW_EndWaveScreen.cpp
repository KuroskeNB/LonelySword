// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_EndWaveScreen.h"
#include "UW_StatsUp.h"
#include "Components/Button.h"

uint8 UUW_EndWaveScreen::GetStatId()
{
      float typeID = FMath::RandRange(0,4);
      while(arrayStats.Contains(typeID))
      {
        typeID = FMath::RandRange(0,4);
      }
      arrayStats.Add(typeID);
      UE_LOG(LogTemp, Display, TEXT("enum type %f"),typeID);
return typeID;
}

void UUW_EndWaveScreen::NativeConstruct()
{
    Super::NativeConstruct();

    if(FirstStat)
    {
      EStatsToUp Type = static_cast<EStatsToUp>(GetStatId());
      FirstStat->LoadButton(Type,*StatTypes.Find(Type),1);
      FirstStat->Button->OnPressed.AddDynamic(this,&UUW_EndWaveScreen::OnStatChoosed);
    }
    if(SecondStat)
    {
      EStatsToUp Type = static_cast<EStatsToUp>(GetStatId());
      SecondStat->LoadButton(Type,*StatTypes.Find(Type),1);   
      SecondStat->Button->OnPressed.AddDynamic(this,&UUW_EndWaveScreen::OnStatChoosed);
    }
    if(ThirdStat)
    {
      EStatsToUp Type = static_cast<EStatsToUp>(GetStatId());
      ThirdStat->LoadButton(Type,*StatTypes.Find(Type),1);
      ThirdStat->Button->OnPressed.AddDynamic(this,&UUW_EndWaveScreen::OnStatChoosed);
    }
}

void UUW_EndWaveScreen::OnStatChoosed()
{
    FInputModeGameOnly GameOnly;
    GetOwningPlayer()->SetInputMode(GameOnly);
    GetOwningPlayer()->SetShowMouseCursor(false);
    RemoveFromParent();
    ConditionalBeginDestroy();
}
