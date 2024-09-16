// Fill out your copyright notice in the Description page of Project Settings.


#include "C_AI_Player.h"
#include "../C_Interface_PlayerCharacter.h"

bool UC_AI_Player::IsPlayerFighting()
{
    if(GetOwningActor() && GetOwningActor()->GetClass()->ImplementsInterface(UC_Interface_PlayerCharacter::StaticClass()))
    {
        if(IC_Interface_PlayerCharacter::Execute_GetPlayerStatus(GetOwningActor())==EPlayerStatus::InBattle)
        {
            return true;
        }
        else if(IC_Interface_PlayerCharacter::Execute_GetPlayerStatus(GetOwningActor())==EPlayerStatus::OutOfBattle)
        {
            return false;
        }
    }
    return false;
}
