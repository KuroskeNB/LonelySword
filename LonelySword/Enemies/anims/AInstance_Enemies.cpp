// Fill out your copyright notice in the Description page of Project Settings.


#include "AInstance_Enemies.h"
#include "../C_Interface_EnemyCharacter.h"


bool UAInstance_Enemies::IsEnemyAlive()
{
    if(GetOwningActor() && GetOwningActor()->GetClass()->ImplementsInterface(UC_Interface_EnemyCharacter::StaticClass()))
    {
        return IC_Interface_EnemyCharacter::Execute_IsAlive(GetOwningActor());
    }
    return false;
}
