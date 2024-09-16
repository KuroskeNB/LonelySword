// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_MainMenu.h"
#include "Blueprint/UserWidget.h"

void APC_MainMenu::BeginPlay()
{
    Super::BeginPlay();
    FInputModeUIOnly UIOnly;
    SetInputMode(UIOnly);
    SetShowMouseCursor(true);
    if(MainMenuClass)
    {
        MainMenuWidget=CreateWidget<UUserWidget>(this,MainMenuClass);
        MainMenuWidget->AddToViewport();
    }
}
