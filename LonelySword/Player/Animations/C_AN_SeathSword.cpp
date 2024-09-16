// Fill out your copyright notice in the Description page of Project Settings.


#include "C_AN_SeathSword.h"
#include "../C_Interface_PlayerCharacter.h"

void UC_AN_SeathSword::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, const FAnimNotifyEventReference &EventReference)
{
 if(MeshComp->GetOwner() &&MeshComp->GetOwner()->GetClass()->ImplementsInterface(UC_Interface_PlayerCharacter::StaticClass()))
 {
    IC_Interface_PlayerCharacter::Execute_LeaveFight(MeshComp->GetOwner());
 }
}
