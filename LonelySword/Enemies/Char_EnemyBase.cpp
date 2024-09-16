// Fill out your copyright notice in the Description page of Project Settings.


#include "Char_EnemyBase.h"
#include "DrawDebugHelpers.h"
#include "../Player/C_Interface_PlayerCharacter.h"
#include "../Tools/C_GS_ArenaWaves.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/SwordDagameTypes.h"
#include "UW_EnemyHealthBar.h"
#include "Components/WidgetComponent.h"

// Sets default values
AChar_EnemyBase::AChar_EnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
    HealthBarComponent->SetupAttachment(RootComponent);
    HealthBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthBarComponent->SetDrawSize(FVector2D(200.0f, 20.0f));
    HealthBarComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f)); // Поднимите виджет над врагом
}

// Called when the game starts or when spawned
void AChar_EnemyBase::BeginPlay()
{
	Super::BeginPlay();
	CharacterData.CurrentHP=CharacterData.MaxHP;
	if (UUW_EnemyHealthBar* HealthBar = Cast<UUW_EnemyHealthBar>(HealthBarComponent->GetUserWidgetObject()))
    {
        HealthBar->SetHealthPercent(CharacterData.CurrentHP / CharacterData.MaxHP);
    }
	 if(Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState()))
    {
        Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState())->OnFinishedWave.AddUObject(this,&AChar_EnemyBase::Die);
    }
}

// Called every frame
void AChar_EnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChar_EnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChar_EnemyBase::CheckDamageForEffects_Implementation(AActor *DamageCauser)
{
	UE_LOG(LogTemp, Display, TEXT("check effect"));
	if(DamageCauser &&DamageCauser->GetClass()->ImplementsInterface(UC_Interface_PlayerCharacter::StaticClass()))
	{
     ASwordDagameTypes* Effect =Cast<ASwordDagameTypes>(IC_Interface_PlayerCharacter::Execute_GetDamageType(DamageCauser));
	 if(Effect)
	 Effect->ImplementEffectTo(this);
	}
}

void AChar_EnemyBase::DropItems_Implementation()
{
 if(!DropItemClass) return;
	for(int i = 0;i<DropItemsCount;i++)
	{
     AActor* item = GetWorld()->SpawnActor<AActor>(DropItemClass);
	 item->SetActorLocation(GetActorLocation());
	}
}

bool AChar_EnemyBase::IsAlive_Implementation()
{
    return bIsAlive;
}

AActor *AChar_EnemyBase::GetDamageType_Implementation()
{
    if(DamageType)
	{
		return GetWorld()->SpawnActor<AActor>(DamageType);
	}
    return nullptr;
}

void AChar_EnemyBase::SetLevelOfPower_Implementation(int level)
{
	//hp
	CharacterData.MaxHP+=(15*level);
	CharacterData.CurrentHP=CharacterData.MaxHP;
	//Damage
	CharacterData.Damage+=(2.5*level);

}

void AChar_EnemyBase::Die(float level)
{
	DropItems_Implementation();
	Destroy();
}

bool AChar_EnemyBase::IsAttacking()
{
   UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance&&!AttackMontage.IsEmpty())
	{
		for(auto Montage : AttackMontage)
		{
			if(AnimInstance->Montage_IsPlaying(Montage))
			return true;
		}
	}
return false;
}

float AChar_EnemyBase::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
    if(!bIsAlive) return 0;
	CharacterData.CurrentHP-=DamageAmount;
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

void AChar_EnemyBase::HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload &Payload)
{
	UE_LOG(LogTemp, Display, TEXT("attack anim notify base"));
}
