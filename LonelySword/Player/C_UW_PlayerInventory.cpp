// Fill out your copyright notice in the Description page of Project Settings.


#include "C_UW_PlayerInventory.h"
#include "Components/WrapBox.h"
#include "Components/Button.h"
#include "C_UW_ItemSlot.h"
#include "Engine/DataTable.h"

void UC_UW_PlayerInventory::NativeConstruct()
{
    Super::NativeConstruct();
    MainWrapBox->GetAllChildren();
}

void UC_UW_PlayerInventory::AddToInventory(FName ItemName, float ItemCount)
{
 if(MainWrapBox && !MainWrapBox->GetAllChildren().IsEmpty())
 {
    for(auto Child : MainWrapBox->GetAllChildren())
    {
        if(Cast<UC_UW_ItemSlot>(Child)&&Cast<UC_UW_ItemSlot>(Child)->SlotData.ResourceName==ItemName)
        {
         Cast<UC_UW_ItemSlot>(Child)->ChangeItemCount(ItemCount);
         return;
        }
    }
    AddSlot(ItemName,ItemCount);
 }
 else if(MainWrapBox && MainWrapBox->GetAllChildren().IsEmpty())
 {
    AddSlot(ItemName,ItemCount);
 }

}

void UC_UW_PlayerInventory::AddSlot(FName ItemName, float ItemCount)
{
    if(MainWrapBox && ResourcesDataTable)
    {
            UC_UW_ItemSlot* Slot1 = CreateWidget<UC_UW_ItemSlot>(this,ItemSlotClass);
            if(Slot1 &&Slot1->SlotData.StaticStruct() && ResourcesDataTable->FindRow<FResourceData>(ItemName,""))
            {
                UE_LOG(LogTemp, Warning, TEXT("add resource: %s"),ItemName);
            Slot1->SlotData=*ResourcesDataTable->FindRow<FResourceData>(ItemName,"");
            Slot1->ChangeItemCount(ItemCount);
            Slot1->UpdateSlotImage();
            MainWrapBox->AddChild(Slot1);
            }
    }
}

void UC_UW_PlayerInventory::UseSlot(int index)
{
    if(!MainWrapBox) return;
    UE_LOG(LogTemp, Display, TEXT("index %f"),index);
    auto SlotArray = MainWrapBox->GetAllChildren();
    if(SlotArray.Num()<index) return;
    Cast<UC_UW_ItemSlot>(SlotArray[index-1])->OnMainButtonClicked();
}
