// Fill out your copyright notice in the Description page of Project Settings.


#include "Char_Fastie.h"
#include "../Player/C_Interface_PlayerCharacter.h"
#include "../Tools/C_GS_ArenaWaves.h"
#include "../Player/SwordDagameTypes.h"
#include "Kismet/GameplayStatics.h"
#include "UW_EnemyHealthBar.h"
#include "Components/WidgetComponent.h"
// Sets default values
AChar_Fastie::AChar_Fastie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChar_Fastie::BeginPlay()
{
	Super::BeginPlay();
	CharacterData.CurrentHP=CharacterData.MaxHP;
	if (UUW_EnemyHealthBar* HealthBar = Cast<UUW_EnemyHealthBar>(HealthBarComponent->GetUserWidgetObject()))
    {
        HealthBar->SetHealthPercent(CharacterData.CurrentHP / CharacterData.MaxHP);
    }
	if(Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState()))
    {
        Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState())->OnFinishedWave.AddUObject(this,&AChar_Fastie::Die);
    }
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this,&AChar_Fastie::HandleOnMontageNotifyBegin);
	}
}

// Called every frame
void AChar_Fastie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChar_Fastie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChar_Fastie::HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload &Payload)
{
	FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.bTraceComplex = true;
    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        GetMesh()->GetBoneLocation(NotifyName),
		GetMesh()->GetBoneLocation(NotifyName),
        FQuat::Identity,
        ECC_GameTraceChannel3,
        FCollisionShape::MakeSphere(60),
        CollisionParams
    );
	if(bHit && HitResult.GetActor()&& HitResult.GetActor()->GetClass()->ImplementsInterface(UC_Interface_PlayerCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Display, TEXT(" fastie beat %s"),*HitResult.GetActor()->GetActorNameOrLabel());
     UGameplayStatics::ApplyDamage(HitResult.GetActor(),CharacterData.Damage,GetController(),this,UDamageType::StaticClass());
	 if(AttackParticle)
	 UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),AttackParticle,GetMesh()->GetBoneLocation(NotifyName));
	}
}

float AChar_Fastie::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
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


void AChar_Fastie::EnemyAttack_Implementation()
{
UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(!AttackMontage.IsEmpty() &&AnimInstance&& !AnimInstance->IsAnyMontagePlaying())
	{
		float MontageIndex=FMath::RandRange(0,AttackMontage.Num()-1);
		PlayAnimMontage(AttackMontage[MontageIndex],2);
	}
}

