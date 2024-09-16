// Fill out your copyright notice in the Description page of Project Settings.


#include "C_AIC_EnemyMutant.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Prediction.h"
#include "Perception/AISense_Prediction.h"
#include "C_Interface_EnemyCharacter.h"
#include "../Player/C_Interface_PlayerCharacter.h"

AC_AIC_EnemyMutant::AC_AIC_EnemyMutant()
{
    SetupPerceptionSystem();
}

void AC_AIC_EnemyMutant::BeginPlay()
{
    Super::BeginPlay();
    if(AIPerceptions)
    {
        AIPerceptions->OnTargetPerceptionUpdated.AddDynamic(this,&AC_AIC_EnemyMutant::OnSight);
    }
    if(GetPawn())
    {
        StartLocation=GetPawn()->GetActorLocation();
    }
}

void AC_AIC_EnemyMutant::SetupPerceptionSystem()
{
    SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>("Perception Component"));
  AIPerceptions=GetPerceptionComponent();
  SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
  PredictionConfig = CreateDefaultSubobject<UAISenseConfig_Prediction>("Prediction Config");
  SightConfig->SetMaxAge(0.0f);
  AIPerceptions->ConfigureSense(*SightConfig);
  AIPerceptions->ConfigureSense(*PredictionConfig);
  AIPerceptions->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AC_AIC_EnemyMutant::OnSight(AActor *Actor, FAIStimulus Stimulus)
{
    if(Actor)
    {
        if(!Actor->GetClass()->ImplementsInterface(UC_Interface_PlayerCharacter::StaticClass())) return;
        if(AttackDistance==0 && GetPawn()->GetClass()->ImplementsInterface(UC_Interface_EnemyCharacter::StaticClass()))
        {
            AttackDistance=IC_Interface_EnemyCharacter::Execute_GetPawnAttackDistance(GetPawn());
        }
        UAISense_Prediction::RequestControllerPredictionEvent(this,Actor,0.1);
        SetFocus(Actor);
        //move
        if(GetPawn()->GetClass()->ImplementsInterface(UC_Interface_EnemyCharacter::StaticClass())&&IC_Interface_EnemyCharacter::Execute_IsAbleToMove(GetPawn())){
         if(AttackDistance>200)
         {MoveToActor(Actor,AttackDistance-150);}
         else{MoveToActor(Actor);}}
        ////attack
        if((FVector::Distance(GetPawn()->GetActorLocation(),Actor->GetActorLocation()))<AttackDistance &&GetPawn()->GetClass()->ImplementsInterface(UC_Interface_EnemyCharacter::StaticClass()))
        {
          IC_Interface_EnemyCharacter::Execute_EnemyAttack(GetPawn());
        }
    }
}
