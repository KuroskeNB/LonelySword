// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_MainMenu.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "../Tools/GI_LonelySword.h"
#include "Kismet/GameplayStatics.h"
#include "../Tools/GameSaves_Arena.h"

void UUW_MainMenu::NativeConstruct()
{
    Super::NativeConstruct();
    if(SetupSwordButton)
    {
        SetupSwordButton->OnPressed.AddDynamic(this,&UUW_MainMenu::OnSetupSwordButtonClicked);
    }
    if(StartGameButton)
    {
       StartGameButton->OnPressed.AddDynamic(this,&UUW_MainMenu::OnStartGameButtonClicked); 
       StartGameButton->SetVisibility(ESlateVisibility::Hidden);
    }
    if(ComboBox)
    {
     UGameSaves_Arena* SaveGameInstance = Cast<UGameSaves_Arena>(UGameplayStatics::LoadGameFromSlot("SaveSlot", 0));
        ComboBox->AddOption("Splash");
        //if(SaveGameInstance &&SaveGameInstance->MaximalLevel>=10)
        ComboBox->AddOption("Freeze");
        //if(SaveGameInstance && SaveGameInstance->MaximalLevel>=5)
        ComboBox->AddOption("Impulse");
        //if(SaveGameInstance&&SaveGameInstance->MaximalLevel>=15)
        ComboBox->AddOption("Poison");
        ComboBox->SetVisibility(ESlateVisibility::Hidden);
        ComboBox->OnSelectionChanged.AddDynamic(this, &UUW_MainMenu::OnSelectionChanged);
    }
}

void UUW_MainMenu::OnSetupSwordButtonClicked()
{
    if(StartGameButton)
    StartGameButton->SetVisibility(ESlateVisibility::Visible);
    if(ComboBox)
    ComboBox->SetVisibility(ESlateVisibility::Visible);
    
    SetupSwordButton->SetVisibility(ESlateVisibility::Hidden);
}

void UUW_MainMenu::OnStartGameButtonClicked()
{
 UGameplayStatics::OpenLevel(GetWorld(),"ArenaLevel");
}

void UUW_MainMenu::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
 if(GetWorld()->GetGameInstance() && Cast<UGI_LonelySword>(GetWorld()->GetGameInstance()))
 {
    Cast<UGI_LonelySword>(GetWorld()->GetGameInstance())->SetSwordDamageType(SelectedItem);
 }
}
