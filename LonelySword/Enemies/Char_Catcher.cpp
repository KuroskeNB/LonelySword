// Fill out your copyright notice in the Description page of Project Settings.


#include "Char_Catcher.h"
#include "../Player/C_Interface_PlayerCharacter.h"
#include "../Tools/C_GS_ArenaWaves.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/SwordDagameTypes.h"
#include "TimerManager.h"
#include "UW_EnemyHealthBar.h"
#include "Components/WidgetComponent.h"
// Sets default values
AChar_Catcher::AChar_Catcher()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChar_Catcher::BeginPlay()
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
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this,&AChar_Catcher::HandleOnMontageNotifyBegin);
	}
	if(Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState()))
    {
        Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState())->OnFinishedWave.AddUObject(this,&AChar_Catcher::Die);
    }
}

// Called every frame
void AChar_Catcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChar_Catcher::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChar_Catcher::HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload &Payload)
{
	UE_LOG(LogTemp, Display, TEXT("notify"));
	FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.bTraceComplex = true;
    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        GetMesh()->GetBoneLocation(NotifyName),
		GetMesh()->GetBoneLocation(NotifyName),
        FQuat::Identity,
        ECC_GameTraceChannel3,
        FCollisionShape::MakeSphere(80),
        CollisionParams
    );
	if(bHit && HitResult.GetActor()&& HitResult.GetActor()->GetClass()->ImplementsInterface(UC_Interface_PlayerCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Display, TEXT("beat %s"),*HitResult.GetActor()->GetActorNameOrLabel());
     UGameplayStatics::ApplyDamage(HitResult.GetActor(),CharacterData.Damage,GetController(),this,UDamageType::StaticClass());
	 CatchedChar = Cast<ACharacter>(HitResult.GetActor());
	 if(CatchedChar)
	 {
		UE_LOG(LogTemp, Warning, TEXT("attach char"));
        CatchedChar->GetCharacterMovement()->MaxWalkSpeed=0;
		CatchedChar->GetCharacterMovement()->bOrientRotationToMovement = false;
		if(AttackParticle)
	 UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),AttackParticle,GetMesh()->GetBoneLocation(NotifyName));
		Catched();
	 }	
	}
}

float AChar_Catcher::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
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
		PlayAnimMontage(ImpactMontage[MontageIndex]);
		if(ImpactParticle)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactParticle,GetActorLocation());
		}
	}
    return 0.0f;
}


void AChar_Catcher::Catched()
{
	if(HoldingMontage[0])
	{
	PlayAnimMontage(HoldingMontage[0],0);
	GetCharacterMovement()->DisableMovement();
	GetWorldTimerManager().SetTimer(ReleafTimer,this,&AChar_Catcher::Releaf,1,false);
	}
}

void AChar_Catcher::Releaf()
{
	if(CatchedChar)
	{
		        CatchedChar->GetCharacterMovement()->MaxWalkSpeed=350;
				GetMesh()->GetAnimInstance()->StopAllMontages(0.1);
				GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void AChar_Catcher::EnemyAttack_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(!AttackMontage.IsEmpty() &&AnimInstance&& !AnimInstance->IsAnyMontagePlaying())
	{
		float MontageIndex=FMath::RandRange(0,AttackMontage.Num()-1);
		PlayAnimMontage(AttackMontage[MontageIndex],1);
	}
}
