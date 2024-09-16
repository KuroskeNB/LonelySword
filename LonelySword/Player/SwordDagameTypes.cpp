// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordDagameTypes.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Enemies/C_Interface_EnemyCharacter.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

void ASwordDagameTypes::ImplementEffectTo(AActor *actor)
{
    switch(DamageType)
    {
        case EDamageType::Impulse:
         Impulse(actor);
         break;
        case EDamageType::Poison:
         Poison(actor);
         break;
        case EDamageType::Freeze:
         Freeze(actor);
         break;
        case EDamageType::Splash:
         Splash(actor);
         break;
    }
}

void ASwordDagameTypes::Freeze(AActor *actor)
{
    ACharacter* Char = Cast<ACharacter>(actor);
    if(!Char) Destroy();
    //freeze
    float StartSpeed =Char->GetCharacterMovement()->MaxWalkSpeed;
    if(StartSpeed==0) {Destroy(); return;}
    Char->GetCharacterMovement()->MaxWalkSpeed=0;
    
    UE_LOG(LogTemp, Display, TEXT("freeze %s"),*actor->GetActorNameOrLabel());
     if(TypeParticle)
	 UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),TypeParticle,actor->GetActorLocation());
    //end freeze
    FTimerHandle FreezeTimer;
    GetWorldTimerManager().SetTimer(FreezeTimer,[this,Char,StartSpeed]()
    {
        if(!Char) this->Destroy();
    Char->GetCharacterMovement()->MaxWalkSpeed=StartSpeed;
    this->Destroy();
    },FreezeDuration,false);
}

void ASwordDagameTypes::Impulse(AActor *actor)
{

    if(actor)
    {
    FVector Direction = -(actor->GetActorForwardVector());
        Direction.Normalize(); // Нормализуем вектор, чтобы получить единичное направление

        // Умножаем нормализованное направление на силу
        FVector Force = Direction * ImpulseForce;
      if(ACharacter* bedolaga = Cast<ACharacter>(actor))
      {
              UE_LOG(LogTemp, Display, TEXT("impulse"));
        bedolaga->LaunchCharacter(Force,true,true);
        if(TypeParticle)
	 UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),TypeParticle,bedolaga->GetActorLocation());
      }
    }
    Destroy();
}

void ASwordDagameTypes::Splash(AActor *actor)
{
    UE_LOG(LogTemp, Display, TEXT("splash begin"));
    TArray<FHitResult> HitResults;
    FCollisionQueryParams CollisionParams;
    CollisionParams.bTraceComplex = true;
    CollisionParams.AddIgnoredActor(actor);
    bool bHit = GetWorld()->SweepMultiByChannel(
        HitResults,
        actor->GetActorLocation(),
		actor->GetActorLocation(),
        FQuat::Identity,
        ECC_GameTraceChannel5,
        FCollisionShape::MakeSphere(SplashRadius),
        CollisionParams);

    if(HitResults.IsEmpty()) 
    {
        UE_LOG(LogTemp, Warning, TEXT("splash no hit"));
    Destroy();
    }
     for(auto res : HitResults)
     {
	 if(bHit && res.GetActor()&& res.GetActor()->GetClass()->ImplementsInterface(UC_Interface_EnemyCharacter::StaticClass()))
	  {
        UE_LOG(LogTemp, Display, TEXT("splash %s"),*res.GetActor()->GetActorNameOrLabel());
        UGameplayStatics::ApplyDamage(res.GetActor(),SplashDamage,nullptr,this,UDamageType::StaticClass());
        if(TypeParticle)
	  UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),TypeParticle,res.GetActor()->GetActorLocation());
	  }
    }
    Destroy();
}

void ASwordDagameTypes::Poison(AActor *actor)
{
    FTimerHandle PoisonTimer;
    GetWorldTimerManager().SetTimer(PoisonTimer,[this,actor,&PoisonTimer](){
        if(!GetWorld()) return;
    if(PoisonIndex>=3 || !actor )
       {
        GetWorldTimerManager().ClearTimer(PoisonTimer);
        Destroy();
        return;
       }
    UGameplayStatics::ApplyDamage(actor,PoisonDamage,nullptr,this,UDamageType::StaticClass());
    UE_LOG(LogTemp, Display, TEXT("poison %s"),*actor->GetActorNameOrLabel());
    if(TypeParticle)
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),TypeParticle,actor->GetActorLocation());
    PoisonIndex++;
    },PoisonInterval,true);
}
