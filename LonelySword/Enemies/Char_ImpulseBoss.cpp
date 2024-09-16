// Fill out your copyright notice in the Description page of Project Settings.


#include "Char_ImpulseBoss.h"
#include "../Player/C_Interface_PlayerCharacter.h"
#include "../Tools/C_GS_ArenaWaves.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/SwordDagameTypes.h"
#include "UW_EnemyHealthBar.h"
#include "Components/WidgetComponent.h"

AChar_ImpulseBoss::AChar_ImpulseBoss()
{
}

void AChar_ImpulseBoss::BeginPlay()
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
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this,&AChar_ImpulseBoss::HandleOnMontageNotifyBegin);
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
        Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState())->OnFinishedWave.AddUObject(this,&AChar_ImpulseBoss::Die);
    }
}

void AChar_ImpulseBoss::EnemyAttack_Implementation()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(!AttackMontage.IsEmpty() &&AnimInstance&& !AnimInstance->IsAnyMontagePlaying())
	{
		if(ImpulseStacks>=3 &&UltimateMontage)
		{
		ImpulseStacks=0;
		bIsAbleToMove=false;
		PlayAnimMontage(UltimateMontage,1);
		return;
		}
		float MontageIndex=FMath::RandRange(0,AttackMontage.Num()-1);
		PlayAnimMontage(AttackMontage[MontageIndex],1);
	}
}

void AChar_ImpulseBoss::Stomp(FName BoneName)
{
    FHitResult HitResult;
 UE_LOG(LogTemp, Display, TEXT("area bone %s"),BoneName);
    FCollisionQueryParams CollisionParams;
    CollisionParams.bTraceComplex = true;
    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        GetMesh()->GetBoneLocation(BoneName),
		GetMesh()->GetBoneLocation(BoneName),
        FQuat::Identity,
        ECC_GameTraceChannel3,
        FCollisionShape::MakeSphere(120),
        CollisionParams
    );
    
	if(bHit && HitResult.GetActor()&& HitResult.GetActor()->GetClass()->ImplementsInterface(UC_Interface_PlayerCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Display, TEXT("beat %s"),*HitResult.GetActor()->GetActorNameOrLabel());
     UGameplayStatics::ApplyDamage(HitResult.GetActor(),CharacterData.Damage,GetController(),this,UDamageType::StaticClass());	
	 ImpulseNotify(HitResult.GetActor());
	}
}

void AChar_ImpulseBoss::Slam(FName BoneName)
{
    FHitResult HitResult;
 UE_LOG(LogTemp, Display, TEXT("area bone %s"),BoneName);
    FCollisionQueryParams CollisionParams;
    CollisionParams.bTraceComplex = true;
    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        GetMesh()->GetBoneLocation(BoneName),
		GetMesh()->GetBoneLocation(BoneName),
        FQuat::Identity,
        ECC_GameTraceChannel3,
        FCollisionShape::MakeSphere(120),
        CollisionParams
    );
    

	if(bHit && HitResult.GetActor()&& HitResult.GetActor()->GetClass()->ImplementsInterface(UC_Interface_PlayerCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Display, TEXT("beat %s"),*HitResult.GetActor()->GetActorNameOrLabel());
     UGameplayStatics::ApplyDamage(HitResult.GetActor(),CharacterData.Damage,GetController(),this,UDamageType::StaticClass());
	 if(AttackParticle)
	 UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),AttackParticle,GetMesh()->GetBoneLocation(BoneName));	
	 ImpulseNotify(HitResult.GetActor());
	}
}

void AChar_ImpulseBoss::Ultimate(FName BoneName)
{
	if(!TargetForUlt) {bIsAbleToMove=true; return;}
	UE_LOG(LogTemp, Warning, TEXT("ultimate on %s"),*TargetForUlt->GetActorNameOrLabel());
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
         UGameplayStatics::ApplyDamage(HitResult.GetActor(),CharacterData.Damage*1.5,GetController(),nullptr,UDamageType::StaticClass());
		 HookActor(HitResult.GetActor());
	     }
		 if(UltimateParticle)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),UltimateParticle,UltLocation);
	bIsAbleToMove=true;
	},0.2,false);
}

void AChar_ImpulseBoss::ImpulseNotify(AActor *Actor)
{
	ImpulseStacks++;
	TargetForUlt=Actor;
	if(ImpulseStacks>=3)
	{
		EnemyAttack_Implementation();
	}
}

void AChar_ImpulseBoss::HookActor(AActor *Actor)
{
	if(ACharacter* Char =Cast<ACharacter>(Actor))
	{
		FVector Direction = -(Actor->GetActorLocation()-GetActorLocation());
        Direction.Normalize();
		Char->LaunchCharacter(Direction*4000,true,true);
	}
}

void AChar_ImpulseBoss::HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload &Payload)
{
 if(NotifyName == "RightFoot")
 {
    Stomp(NotifyName);
 }
 else if(NotifyName == "RightHand")
 {
   Slam(NotifyName);
 }
 else if(NotifyName == "LeftHand")
 {
  Slam(NotifyName);
 }
 else if(NotifyName == "Head")
 {
  Ultimate(NotifyName);
 }
}

float AChar_ImpulseBoss::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
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
