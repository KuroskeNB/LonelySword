// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_EnemyHealthBar.h"
#include "Components/ProgressBar.h"

void UUW_EnemyHealthBar::SetHealthPercent(float Percent)
{
 if (HealthProgressBar)
    {
        HealthProgressBar->SetPercent(Percent);
    }
}
