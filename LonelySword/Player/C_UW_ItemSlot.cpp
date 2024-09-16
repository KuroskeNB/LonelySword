// Fill out your copyright notice in the Description page of Project Settings.


#include "C_UW_ItemSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "C_Interface_PlayerCharacter.h"
#include "TimerManager.h"

void UC_UW_ItemSlot::NativeConstruct()
{
    Super::NativeConstruct();
    if(MainButton)
    {
     MainButton->OnPressed.AddDynamic(this,&UC_UW_ItemSlot::OnMainButtonClicked);
    }
    if(UseButton)
    {
     UseButton->SetVisibility(ESlateVisibility::Hidden);
     UseButton->OnPressed.AddDynamic(this,&UC_UW_ItemSlot::OnUseButtonClicked);
    }
    if(DropButton)
    {
        DropButton->SetVisibility(ESlateVisibility::Hidden);
        DropButton->OnPressed.AddDynamic(this,&UC_UW_ItemSlot::OnDropButtonClicked);
    }
    if(CountText)
    {
        CountText->SetText(FText::FromString(FString::SanitizeFloat(ItemCount)));
    }
}

void UC_UW_ItemSlot::ChangeItemCount(float NewCount)
{
    if(CountText)
    {
        UE_LOG(LogTemp, Display, TEXT("change count"));
        ItemCount+=NewCount;
        if(ItemCount<=0)
        {
            RemoveFromParent();
            ConditionalBeginDestroy();
        }
        CountText->SetText(FText::FromString(FString::Printf(TEXT("%.f"),ItemCount)));;
    }
}

void UC_UW_ItemSlot::UpdateSlotImage()
{
    if(MainButton)
    {
        FButtonStyle ButtonStyle = MainButton->GetStyle();
        FSlateBrush ImageBrush;
    
     ImageBrush.SetResourceObject(SlotData.Icon);


        ButtonStyle.Normal = ImageBrush;
        //ButtonStyle.Hovered = ImageBrush;
        ButtonStyle.Pressed = ImageBrush;
    MainButton->SetStyle(ButtonStyle);
    }
}

void UC_UW_ItemSlot::OnMainButtonClicked()
{
    if(!bCanBeUsed) return;

   if(GetOwningPlayerPawn() && GetOwningPlayerPawn()->GetClass()->ImplementsInterface(UC_Interface_PlayerCharacter::StaticClass()))
   {
    IC_Interface_PlayerCharacter::Execute_DrinkPotion(GetOwningPlayerPawn(),SlotData.type);
    ChangeItemCount(-1);
    bCanBeUsed=false;
    GetWorld()->GetTimerManager().SetTimer(Cooldown,[this](){bCanBeUsed=true;},SlotData.CooldownTime,false);
   }

    if(UseButton)
    {
     UseButton->SetVisibility(ESlateVisibility::Visible);
    }
    if(DropButton)
    {
     DropButton->SetVisibility(ESlateVisibility::Visible);
    }
}

void UC_UW_ItemSlot::OnUseButtonClicked()
{
}

void UC_UW_ItemSlot::OnDropButtonClicked()
{
}
