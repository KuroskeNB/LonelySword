// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_LonelySword.h"

void UGI_LonelySword::SetSwordDamageType(FString TypeName)
{
    if (TypeName == "Freeze")
    {
        SwordDamageType=FreezeClass;
    }
    else if (TypeName == "Impulse")
    {
        SwordDamageType=ImpulseClass;
    }
    else if (TypeName == "Splash")
    {
       SwordDamageType=SplashClass;
    }
    else if (TypeName == "Poison")
    {
        SwordDamageType=PoisonClass;
    }
}
