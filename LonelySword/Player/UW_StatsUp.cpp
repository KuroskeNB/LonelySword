// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_StatsUp.h"
#include "C_Interface_PlayerCharacter.h"
#include "Components/Button.h"

void UUW_StatsUp::NativeConstruct()
{
    Super::NativeConstruct();
}

void UUW_StatsUp::LoadButton(EStatsToUp stat, UTexture2D *texture, float lvl)
{
    if(Button)
    {
        Level=lvl;
        Stat=stat;
        Button->OnPressed.AddDynamic(this,&UUW_StatsUp::OnButtonClicked);

        // texture
        if(!texture) return;
        FButtonStyle ButtonStyle = Button->GetStyle();
        FSlateBrush ImageBrush;
    
     ImageBrush.SetResourceObject(texture);


        ButtonStyle.Normal = ImageBrush;
        ButtonStyle.Pressed = ImageBrush;
    Button->SetStyle(ButtonStyle);
    }
}

void UUW_StatsUp::OnButtonClicked()
{
    if(GetOwningPlayerPawn()->GetClass()->ImplementsInterface(UC_Interface_PlayerCharacter::StaticClass()))
    {
        IC_Interface_PlayerCharacter::Execute_UpStat(GetOwningPlayerPawn(),Stat,Level);
        UE_LOG(LogTemp, Display, TEXT("clicked"));
    }
}
