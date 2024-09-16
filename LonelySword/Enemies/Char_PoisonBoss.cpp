// Fill out your copyright notice in the Description page of Project Settings.


#include "Char_PoisonBoss.h"
#include "../Player/C_Interface_PlayerCharacter.h"
#include "../Tools/C_GS_ArenaWaves.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/SwordDagameTypes.h"
#include "UW_EnemyHealthBar.h"
#include "Components/WidgetComponent.h"
AChar_PoisonBoss::AChar_PoisonBoss()
{
}

void AChar_PoisonBoss::BeginPlay()
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
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this,&AChar_PoisonBoss::HandleOnMontageNotifyBegin);
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
        Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState())->OnFinishedWave.AddUObject(this,&AChar_PoisonBoss::Die);
    }
}

void AChar_PoisonBoss::EnemyAttack_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(!AttackMontage.IsEmpty() &&AnimInstance&& !AnimInstance->IsAnyMontagePlaying())
	{
		if(PoisonStacks>=4 &&UltimateMontage)
		{
		PoisonStacks=0;
		bIsAbleToMove=false;
		PlayAnimMontage(UltimateMontage,1);
		return;
		}
		//float MontageIndex=FMath::RandRange(0,AttackMontage.Num()-1);
		if(AttackIndex<2)
		{
		PlayAnimMontage(AttackMontage[0],1);
		AttackIndex++;
		}
		else if(AttackMontage.Num()>1&&AttackIndex==2)
		{
          PlayAnimMontage(AttackMontage[1],1);
		  AttackIndex=0;
		}
	}
}

void AChar_PoisonBoss::AreaAttack(FName BoneName)
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
        FCollisionShape::MakeSphere(CharacterData.AttackDistance),
        CollisionParams
    );
    
	if(AreaParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),AreaParticle,GetMesh()->GetBoneLocation(BoneName),GetActorRotation());
	}

	if(bHit && HitResult.GetActor()&& HitResult.GetActor()->GetClass()->ImplementsInterface(UC_Interface_PlayerCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Display, TEXT("beat %s"),*HitResult.GetActor()->GetActorNameOrLabel());
     UGameplayStatics::ApplyDamage(HitResult.GetActor(),CharacterData.Damage,GetController(),this,UDamageType::StaticClass());	
	 PoisonNotify(HitResult.GetActor());
	}
}

void AChar_PoisonBoss::Ultimate(FName BoneName)
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
         UGameplayStatics::ApplyDamage(HitResult.GetActor(),CharacterData.Damage*1.5,GetController(),this,UDamageType::StaticClass());
	     }
		 bIsAbleToMove=true;
		 if(UltimateParticle)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),UltimateParticle,UltLocation);
	},0.2,false);
}

void AChar_PoisonBoss::CloudAttack(FName BoneName)
{
	FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.bTraceComplex = true;
    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        GetMesh()->GetBoneLocation(BoneName),
		GetMesh()->GetBoneLocation(BoneName)+GetActorRotation().Vector()*400,
        FQuat::Identity,
        ECC_GameTraceChannel3,
        FCollisionShape::MakeCapsule(60,200),
        CollisionParams
    );
    
	if(CloudParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),CloudParticle,GetMesh()->GetBoneLocation(BoneName),GetActorRotation());
	}

	if(bHit && HitResult.GetActor()&& HitResult.GetActor()->GetClass()->ImplementsInterface(UC_Interface_PlayerCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Display, TEXT("beat %s"),*HitResult.GetActor()->GetActorNameOrLabel());
     UGameplayStatics::ApplyDamage(HitResult.GetActor(),CharacterData.Damage,GetController(),this,UDamageType::StaticClass());
	 PoisonNotify(HitResult.GetActor());
	}
}

void AChar_PoisonBoss::HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload &Payload)
{
if(NotifyName=="RightHand")
{
	CloudAttack(NotifyName);
}
else if(NotifyName=="Spine")
{
	AreaAttack(NotifyName);
}
else if(NotifyName=="Head")
{
	Ultimate(NotifyName);
}
}

float AChar_PoisonBoss::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
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

void AChar_PoisonBoss::PoisonNotify(AActor *Actor)
{
	PoisonStacks++;
	if(UltimateMontage)
	{
		TargetForUlt=Actor;
	}
}
