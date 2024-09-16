// Fill out your copyright notice in the Description page of Project Settings.


#include "Char_FreezeBoss.h"
#include "../Player/C_Interface_PlayerCharacter.h"
#include "../Tools/C_GS_ArenaWaves.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/SwordDagameTypes.h"
#include "TimerManager.h"
#include "UW_EnemyHealthBar.h"
#include "Components/WidgetComponent.h"

AChar_FreezeBoss::AChar_FreezeBoss()
{
}

void AChar_FreezeBoss::BeginPlay()
{
     Super::BeginPlay();
	CharacterData.CurrentHP=CharacterData.MaxHP;
	if (UUW_EnemyHealthBar* HealthBar = Cast<UUW_EnemyHealthBar>(HealthBarComponent->GetUserWidgetObject()))
    {
        HealthBar->SetHealthPercent(CharacterData.CurrentHP / CharacterData.MaxHP);
    }
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this,&AChar_FreezeBoss::HandleOnMontageNotifyBegin);
		// if(DeathMontage&&DeathState)
		// {
		// 	UE_LOG(LogTemp, Display, TEXT("bind montage end"));
		// 	FOnMontageEnded OnMontageEnded;
		// 	OnMontageEnded.BindUObject(this,&AC_EnemyCharacter::EnterDeathState);
		// 	AnimInstance->Montage_SetEndDelegate(OnMontageEnded,DeathMontage);
		// }
	}
	 if(Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState()))
    {
        Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState())->OnFinishedWave.AddUObject(this,&AChar_FreezeBoss::Die);
    }
}

void AChar_FreezeBoss::EnemyAttack_Implementation()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(!AttackMontage.IsEmpty() &&AnimInstance&& !AnimInstance->IsAnyMontagePlaying())
	{
		if(FreezeStacks==3 &&UltimateMontage)
		{
		FreezeStacks=0;
		bIsAbleToMove=false;
		PlayAnimMontage(UltimateMontage,1);
		return;
		}
		UE_LOG(LogTemp, Display, TEXT("bFreezed %u"),bFreezed);
		if(!bFreezed)
		{
		float MontageIndex=FMath::RandRange(0,AttackMontage.Num()-2);
		PlayAnimMontage(AttackMontage[MontageIndex],1);
		}
		else if(bFreezed&&AttackMontage.Num()>2)
		{
		 PlayAnimMontage(AttackMontage[2],1);
		}
	}
}


float AChar_FreezeBoss::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
   if(!bIsAlive) return 0;
	CharacterData.CurrentHP-=DamageAmount;
	if (UUW_EnemyHealthBar* HealthBar = Cast<UUW_EnemyHealthBar>(HealthBarComponent->GetUserWidgetObject()))
    {
        HealthBar->SetHealthPercent(CharacterData.CurrentHP / CharacterData.MaxHP);
    }
	if(CharacterData.CurrentHP<=0)
	{
		if(DeathMontage)
		PlayAnimMontage(DeathMontage);
		DetachFromControllerPendingDestroy();
        Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState())->MobDied();
		SetActorEnableCollision(false);
		bIsAlive = false;
	}
	CheckDamageForEffects_Implementation(DamageCauser);
	
    if(!GetMesh()) return 0;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	
	if(!ImpactMontage.IsEmpty()&&AnimInstance &&(!AnimInstance->IsAnyMontagePlaying() ||IsAttacking()))
	{   
		float rand = FMath::RandRange(1,3);
		if(rand==2)
		{
		float MontageIndex=FMath::RandRange(0,ImpactMontage.Num()-1);
		if(ImpactMontage[MontageIndex])
		PlayAnimMontage(ImpactMontage[MontageIndex]);
		if(ImpactParticle)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactParticle,GetActorLocation());
		}
	}
    return 0.0f;
}

void AChar_FreezeBoss::HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload &Payload)
{
 if(NotifyName=="RightFoot" || NotifyName=="LeftFoot")
 {
	FootAttack(NotifyName);
 }
 else if(NotifyName=="RightHand" || NotifyName=="LeftHand")
 {
	FistCombo(NotifyName);
 }
 else if(NotifyName == "Head")
 {
  Ultimate(NotifyName);
 }
}

void AChar_FreezeBoss::Ultimate(FName BoneName)
{
	UE_LOG(LogTemp, Warning, TEXT("ultimate on %s"),*TargetForUlt->GetActorNameOrLabel());
	if(!TargetForUlt) {bIsAbleToMove=true; return;}
	FVector UltLocation=TargetForUlt->GetActorLocation();
	TargetForUlt=nullptr;
	FTimerHandle UltTimer;
	GetWorldTimerManager().SetTimer(UltTimer,[this,UltLocation]()
	{
       FHitResult HitResult;
       FCollisionQueryParams CollisionParams;
       CollisionParams.bTraceComplex = true;
       bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        UltLocation,
		UltLocation,
        FQuat::Identity,
        ECC_GameTraceChannel3,
        FCollisionShape::MakeSphere(120),
        CollisionParams
        );
	    if(bHit && HitResult.GetActor()&& HitResult.GetActor()->GetClass()->ImplementsInterface(UC_Interface_PlayerCharacter::StaticClass()))
	     {
		 UE_LOG(LogTemp, Display, TEXT("ult %s"),*HitResult.GetActor()->GetActorNameOrLabel());
         UGameplayStatics::ApplyDamage(HitResult.GetActor(),CharacterData.Damage*1.5,GetController(),this,UDamageType::StaticClass());
		bIsAbleToMove=true;
	     }
		 if(UltimateParticle)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),UltimateParticle,HitResult.GetActor()->GetActorLocation());
	},0.15,false);
}

void AChar_FreezeBoss::FootAttack(FName BoneName)
{
	FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.bTraceComplex = true;
    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        GetMesh()->GetBoneLocation(BoneName),
		GetMesh()->GetBoneLocation(BoneName),
        FQuat::Identity,
        ECC_GameTraceChannel3,
        FCollisionShape::MakeSphere(60),
        CollisionParams
    );
	if(bHit && HitResult.GetActor()&& HitResult.GetActor()->GetClass()->ImplementsInterface(UC_Interface_PlayerCharacter::StaticClass()))
	{
     UGameplayStatics::ApplyDamage(HitResult.GetActor(),CharacterData.Damage,GetController(),this,UDamageType::StaticClass());
	 if(AttackParticle)
	 UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),AttackParticle,GetMesh()->GetBoneLocation(BoneName));
	 FreezeNotify(HitResult.GetActor());
	}
}

void AChar_FreezeBoss::FistCombo(FName BoneName)
{
	FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.bTraceComplex = true;
    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        GetMesh()->GetBoneLocation(BoneName),
		GetMesh()->GetBoneLocation(BoneName),
        FQuat::Identity,
        ECC_GameTraceChannel3,
        FCollisionShape::MakeSphere(60),
        CollisionParams
    );
	if(bHit && HitResult.GetActor()&& HitResult.GetActor()->GetClass()->ImplementsInterface(UC_Interface_PlayerCharacter::StaticClass()))
	{
     UGameplayStatics::ApplyDamage(HitResult.GetActor(),CharacterData.Damage,GetController(),nullptr,UDamageType::StaticClass());
	 if(AttackParticle)
	 UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),AttackParticle,GetMesh()->GetBoneLocation(BoneName));
	}
}

void AChar_FreezeBoss::FreezeNotify(AActor* Actor)
{
	bFreezed=true;
	FreezeStacks++;
	if(UltimateMontage)
	{
		TargetForUlt=Actor;
	}
	UE_LOG(LogTemp, Display, TEXT("freeze %u"),bFreezed);
 FTimerHandle FreezeFinish;
 GetWorld()->GetTimerManager().SetTimer(FreezeFinish,[this](){bFreezed=false;},1.2,false);
}
