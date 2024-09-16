// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_DeathScreen.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "../Tools/GI_LonelySword.h"
#include "Kismet/GameplayStatics.h"
#include "../Tools/GameSaves_Arena.h"

void UUW_DeathScreen::NativeConstruct()
{
    Super::NativeConstruct();
    if(RestartGameButton)
    {
        RestartGameButton->OnPressed.AddDynamic(this,&UUW_DeathScreen::OnRestartGameClicked);
    }
    if(StartGameButton)
    {
        StartGameButton->OnPressed.AddDynamic(this,&UUW_DeathScreen::OnStartGameClicked);
        StartGameButton->SetVisibility(ESlateVisibility::Hidden);
    }
    if(ExitButton)
    {
        ExitButton->OnPressed.AddDynamic(this,&UUW_DeathScreen::OnExitClicked);
        
    }
    if(ComboBox)
    {
        UGameSaves_Arena* SaveGameInstance = Cast<UGameSaves_Arena>(UGameplayStatics::LoadGameFromSlot("SaveSlot", 0));
        ComboBox->AddOption("Splash");
        if(SaveGameInstance &&SaveGameInstance->MaximalLevel>=10)
        ComboBox->AddOption("Freeze");
        if(SaveGameInstance && SaveGameInstance->MaximalLevel>=5)
        ComboBox->AddOption("Impulse");
        if(SaveGameInstance&&SaveGameInstance->MaximalLevel>=15)
        ComboBox->AddOption("Poison");
        ComboBox->SetVisibility(ESlateVisibility::Hidden);
        ComboBox->OnSelectionChanged.AddDynamic(this, &UUW_DeathScreen::OnSelectionChanged);
    }
}

void UUW_DeathScreen::OnRestartGameClicked()
{
    if(StartGameButton)
    StartGameButton->SetVisibility(ESlateVisibility::Visible);
    if(ComboBox)
    ComboBox->SetVisibility(ESlateVisibility::Visible);
    
    RestartGameButton->SetVisibility(ESlateVisibility::Hidden);
}

void UUW_DeathScreen::OnStartGameClicked()
{
    UGameplayStatics::OpenLevel(GetWorld(),"ArenaLevel");
}

void UUW_DeathScreen::OnExitClicked()
{

}

void UUW_DeathScreen::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if(GetWorld()->GetGameInstance() && Cast<UGI_LonelySword>(GetWorld()->GetGameInstance()))
 {
    Cast<UGI_LonelySword>(GetWorld()->GetGameInstance())->SetSwordDamageType(SelectedItem);
 }
}
