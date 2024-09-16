// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "C_PC_PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "../Tools/GI_LonelySword.h"
#include "../Enemies/C_Interface_EnemyCharacter.h"
#include "../Tools/C_GS_ArenaWaves.h"
#include "../Tools/GameSaves_Arena.h"

// Sets default values

void AC_PlayerCharacter::StartEnergyRegen()
{

	GetWorldTimerManager().SetTimer(EnergyCd,[this]()
	{
     CharacterData.CurrentEnergy+=5;
	 Cast<AC_PC_PlayerCharacter>(Controller)->PawnEnergyChanged(0.05f);
	 if(CharacterData.CurrentEnergy>=100)
	 GetWorldTimerManager().ClearTimer(EnergyCd);
	},0.1,true);
}

bool AC_PlayerCharacter::IsAttacking()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && !AttackComboAnim.IsEmpty())
	{
		for(auto Montage : AttackComboAnim)
		{
			if(AnimInstance->Montage_IsPlaying(Montage))
			return true;
		}
	}
	if(AnimInstance && !AttackAnims.IsEmpty())
	{
	for(auto item:AttackAnims)
	{
		if(AnimInstance->Montage_IsPlaying(item))
			return true;
	}
	}
    return false;
}

AC_PlayerCharacter::AC_PlayerCharacter() : ALonelySwordCharacter()
{

}

// Called when the game starts or when spawned
void AC_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
    CharacterData.CurrentHP=CharacterData.MaxHP;
	CharacterData.CurrentEnergy=CharacterData.MaxEnergy;
    if(Cast<AC_PC_PlayerCharacter>(Controller))
	Cast<AC_PC_PlayerCharacter>(Controller)->PawnDrainingUpdate(CharacterData.Damage,CharacterData.CurrentHP,CharacterData.MaxHP);
	StartLvlHP=CharacterData.MaxHP;
	StartLvlDamage=CharacterData.Damage;
	if(SwordClass)
	{
		SwordPtr=GetWorld()->SpawnActor<AActor>(SwordClass);
		SwordPtr->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,TEXT("SwordSocket1"));
		SwordPtr->OnActorBeginOverlap.AddDynamic(this,&AC_PlayerCharacter::OnSwordOverlapBegin);
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this,&AC_PlayerCharacter::HandleOnMontageNotifyBegin);
	}
	if(GetWorld()->GetGameInstance()&&Cast<UGI_LonelySword>(GetWorld()->GetGameInstance()))
	{
		DamageType=Cast<UGI_LonelySword>(GetWorld()->GetGameInstance())->GetDamageType();
	}
}



/// FIGHTING ///////////////////////////////////////////////////////////////////
void AC_PlayerCharacter::SwordAttack()
{
	if(!AttackComboAnim.IsEmpty() && CurrentStatus==EPlayerStatus::InBattle && !IsAttacking()&&CharacterData.CurrentEnergy>=15)
	{
       if(AttackAnims.Num()>=2)
	   AttackAnims.Swap(0,1);

		GetWorldTimerManager().ClearTimer(EnergyCd);
		//float MontageIndex=FMath::RandRange(0,AttackComboAnim.Num()-1);
		//PlayAnimMontage(AttackComboAnim[MontageIndex],CharacterData.AttackSpeed,NAME_None);
        PlayAnimMontage(AttackAnims[AttackAnimsIndex],CharacterData.AttackSpeed,NAME_None);

		CharacterData.CurrentEnergy-=12/CharacterData.EnergyMultiplier;
		Cast<AC_PC_PlayerCharacter>(Controller)->PawnEnergyChanged(-0.12f/CharacterData.EnergyMultiplier);
		GetWorldTimerManager().SetTimer(EnergyCd,this,&AC_PlayerCharacter::StartEnergyRegen,1,false);
	}
	else if(IsAttacking())
	{
		AttackIndex=1;
	}
	else if(DrawSwordAnim &&CurrentStatus==EPlayerStatus::OutOfBattle)
	{
		PlayAnimMontage(DrawSwordAnim,1.0,NAME_None);
	}
}

void AC_PlayerCharacter::EndBattle()
{
	if(SeathSwordAnim)
	{
		PlayAnimMontage(SeathSwordAnim,1.0,NAME_None);
	}
}

void AC_PlayerCharacter::Strafe()
{
	if(GetCharacterMovement()&&GetMesh()->GetAnimInstance() &&!GetMesh()->GetAnimInstance()->Montage_IsPlaying(StrafeAnim)&& CharacterData.CurrentEnergy>=25 && GetCharacterMovement()->MaxWalkSpeed>0)
	{
		GetWorldTimerManager().ClearTimer(EnergyCd);
		GetMesh()->GetAnimInstance()->StopAllMontages(0.1f);
		if(StrafeAnim)
		PlayAnimMontage(StrafeAnim,1);
		LaunchCharacter(GetCharacterMovement()->GetLastInputVector()*1750,true,true);
		CharacterData.CurrentEnergy-=20/CharacterData.EnergyMultiplier;
		Cast<AC_PC_PlayerCharacter>(Controller)->PawnEnergyChanged(-0.2f/CharacterData.EnergyMultiplier);
		GetWorldTimerManager().SetTimer(EnergyCd,this,&AC_PlayerCharacter::StartEnergyRegen,1,false);
		AttackAnimsIndex=0;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////

///// INVENTORY /////////////////////////////////////////////////////
void AC_PlayerCharacter::OpenInventory()
{
 if(Cast<AC_PC_PlayerCharacter>(GetController()))
   Cast<AC_PC_PlayerCharacter>(GetController())->OpenInventory();
}

void AC_PlayerCharacter::PickupItem()
{
	AC_PC_PlayerCharacter* PController= Cast<AC_PC_PlayerCharacter>(GetController());
  if(PController)
  {
	 FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.bTraceComplex = true;

    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        GetActorLocation(),
		GetActorLocation(),
        FQuat::Identity,
        ECC_GameTraceChannel2,
        FCollisionShape::MakeSphere(80),
        CollisionParams
    );
	if(bHit && HitResult.GetActor())
	{
		PController->AddPickedToInventory(HitResult.GetActor());
	}
  }
}
void AC_PlayerCharacter::UseSlot(float index)
{
	if(index<1 || previousindex==index) return;
	
	if(Cast<AC_PC_PlayerCharacter>(GetController()))
   Cast<AC_PC_PlayerCharacter>(GetController())->PlayerUsedSlot(index);
   previousindex=index;
}
void AC_PlayerCharacter::ShowCursor()
{

	if(!bCursorIsShown)
	{
		Cast<APlayerController>(GetController())->SetShowMouseCursor(true);
		FInputModeGameAndUI UIOnly;
		Cast<APlayerController>(GetController())->SetInputMode(UIOnly);
		bCursorIsShown=true;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("alt released"));
		Cast<APlayerController>(GetController())->SetShowMouseCursor(false);
		FInputModeGameOnly GameOnly;
		Cast<APlayerController>(GetController())->SetInputMode(GameOnly);
		bCursorIsShown=false;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// DRAINING//////////////////////////////////////////////////////////////////////////////////////

void AC_PlayerCharacter::StartBoostDurability()
{
	if(DurabilityBoostPercent>=PercentMaxCapacity) return;
	GetWorldTimerManager().SetTimer(DrainingTimer,[this]()
	{
		if(CharacterData.Damage<=5)GetWorldTimerManager().ClearTimer(DrainingTimer);
		if(DurabilityBoostPercent>=PercentMaxCapacity) GetWorldTimerManager().ClearTimer(DrainingTimer);
		DurabilityBoostPercent+=(DurabilityBoostPercent>=0)?2.5 : 1.25;
	   MasteryBoostPercent-=(MasteryBoostPercent>0)?2.5:1.25;
       UE_LOG(LogTemp, Display, TEXT("percents mastery %f durability %f"),MasteryBoostPercent,DurabilityBoostPercent);
		//HealthChange
	 float Difference = CharacterData.MaxHP/CharacterData.CurrentHP;
     CharacterData.MaxHP=StartLvlHP*(1+DurabilityBoostPercent/100);
	 CharacterData.CurrentHP=CharacterData.MaxHP/Difference;
	 Cast<AC_PC_PlayerCharacter>(Controller)->PawnHealthChanged(CharacterData.CurrentHP/CharacterData.MaxHP);

	 //DamageChange
	 CharacterData.Damage=StartLvlDamage*(1+(MasteryBoostPercent)/100);

	 UE_LOG(LogTemp, Display, TEXT("Your damage %f and hp %f"),CharacterData.Damage,CharacterData.CurrentHP);
	 Cast<AC_PC_PlayerCharacter>(Controller)->PawnDrainingUpdate(CharacterData.Damage,CharacterData.CurrentHP,CharacterData.MaxHP);
	 if(DurabilityBoostPercent>=PercentMaxCapacity) GetWorldTimerManager().ClearTimer(DrainingTimer);
	},0.025,true);
}

void AC_PlayerCharacter::EndBoostDurability()
{
	GetWorldTimerManager().ClearTimer(DrainingTimer);
}

void AC_PlayerCharacter::StartBoostMastery()
{
	if(MasteryBoostPercent>=PercentMaxCapacity) return;
	GetWorldTimerManager().SetTimer(DrainingTimer,[this]()
	{
		if(MasteryBoostPercent>=PercentMaxCapacity) GetWorldTimerManager().ClearTimer(DrainingTimer);
		float Difference = CharacterData.MaxHP/CharacterData.CurrentHP;
		if(CharacterData.CurrentHP<=2){GetWorldTimerManager().ClearTimer(DrainingTimer);return;}
		DurabilityBoostPercent-=(DurabilityBoostPercent>0)?2.5 : 1.25;
	   MasteryBoostPercent+=(MasteryBoostPercent>=0)?2.5:1.25;
       
	   UE_LOG(LogTemp, Display, TEXT("percents mastery %f durability %f"),MasteryBoostPercent,DurabilityBoostPercent);
		//HealthChange
     CharacterData.MaxHP=StartLvlHP*(1+(DurabilityBoostPercent)/100);
	 CharacterData.CurrentHP=CharacterData.MaxHP/Difference;
	 Cast<AC_PC_PlayerCharacter>(Controller)->PawnHealthChanged(CharacterData.CurrentHP/CharacterData.MaxHP);

	 //DamageChange
	 CharacterData.Damage=StartLvlDamage*(1+MasteryBoostPercent/100);

	 UE_LOG(LogTemp, Display, TEXT("Your damage %f and hp %f"),CharacterData.Damage,CharacterData.CurrentHP);

	 Cast<AC_PC_PlayerCharacter>(Controller)->PawnDrainingUpdate(CharacterData.Damage,CharacterData.CurrentHP,CharacterData.MaxHP);
	 if(MasteryBoostPercent>=PercentMaxCapacity) GetWorldTimerManager().ClearTimer(DrainingTimer);
	},0.025,true);
}

void AC_PlayerCharacter::EndBoostMastery()
{
	GetWorldTimerManager().ClearTimer(DrainingTimer);
}

void AC_PlayerCharacter::HealthChange(float change)
{
	if(CharacterData.CurrentHP+change>CharacterData.MaxHP)
	change = CharacterData.MaxHP-CharacterData.CurrentHP;
	CharacterData.CurrentHP+=change;
	//StartLvlHP+=change;
	if(CharacterData.CurrentHP<CharacterData.MaxHP)
	Cast<AC_PC_PlayerCharacter>(Controller)->PawnHealthChanged(CharacterData.CurrentHP/CharacterData.MaxHP);
	Cast<AC_PC_PlayerCharacter>(Controller)->PawnDrainingUpdate(CharacterData.Damage,CharacterData.CurrentHP,CharacterData.MaxHP);

}

void AC_PlayerCharacter::Death()
{
	UE_LOG(LogTemp, Warning, TEXT("death"));
	FTimerHandle GameEndTimer;
	GetWorldTimerManager().SetTimer(GameEndTimer,[this](){UGameplayStatics::SetGamePaused(GetWorld(),true);},2.5,false);
	Cast<AC_PC_PlayerCharacter>(Controller)->OpenMenu();
	SaveLevel();
}

void AC_PlayerCharacter::FullRecover()
{


}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


////////// DAMAGE ///////////////////////////////////////////////////////////////
float AC_PlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	HealthChange(-DamageAmount);
    //////////////////////
    CheckDamageForEffects(DamageCauser);

	if (ImpactSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
    }
	if(CharacterData.CurrentHP<=0)
	{
     Death();
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(!ImpactMontage.IsEmpty()&&AnimInstance &&(!AnimInstance->IsAnyMontagePlaying() ||IsAttacking()))
	{
		float MontageIndex=FMath::RandRange(0,ImpactMontage.Num()-1);
		PlayAnimMontage(ImpactMontage[MontageIndex]);
		if(ImpactParticle)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactParticle,GetActorLocation());

		AttackAnimsIndex=0;
	}
    return 0.0f;
}

void AC_PlayerCharacter::OnSwordOverlapBegin(AActor *OverlappedActor, AActor *OtherActor)
{
	if(Cast<APawn>(OtherActor)&&OtherActor!=this && IsAttacking())
	{
		UE_LOG(LogTemp, Display, TEXT("sword overlap"));
     UGameplayStatics::ApplyDamage(OtherActor,CharacterData.Damage,GetController(),this,UDamageType::StaticClass());
     if (SwordSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, SwordSound, GetActorLocation());
    }
	 //vampirism
	 if(CharacterData.Vampirism>0)
	 {
	 HealthChange((CharacterData.Damage/100)*CharacterData.Vampirism);
	 }

	}
}
void AC_PlayerCharacter::CheckDamageForEffects(AActor *DamageCauser)
{
	if(DamageCauser &&DamageCauser->GetClass()->ImplementsInterface(UC_Interface_EnemyCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Display, TEXT("check effect"));
     ASwordDagameTypes* Effect =Cast<ASwordDagameTypes>(IC_Interface_EnemyCharacter::Execute_GetDamageType(DamageCauser));
	 if(Effect)
	 Effect->ImplementEffectTo(this);
	}
}
void AC_PlayerCharacter::SaveLevel()
{
	if(Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState()))
	{
		UGameSaves_Arena* SaveGameInstance = Cast<UGameSaves_Arena>(UGameplayStatics::CreateSaveGameObject(UGameSaves_Arena::StaticClass()));
		if(SaveGameInstance)
		{
        SaveGameInstance->MaximalLevel=Cast<AC_GS_ArenaWaves>(GetWorld()->GetGameState())->GetCurrentLevel();
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, "SaveSlot", 0);
		}
	}
}
///////////////////////////////////////////////////////////////

// Called every frame
void AC_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AC_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAction("SwordAttack",IE_Pressed,this,&AC_PlayerCharacter::SwordAttack);
	PlayerInputComponent->BindAction("Strafe",IE_Pressed,this,&AC_PlayerCharacter::Strafe);
	PlayerInputComponent->BindAction("SeathSword",IE_Pressed,this,&AC_PlayerCharacter::EndBattle);
	PlayerInputComponent->BindAction("OpenInventory",IE_Pressed,this,&AC_PlayerCharacter::OpenInventory);
	PlayerInputComponent->BindAction("PickupItem",IE_Pressed,this,&AC_PlayerCharacter::PickupItem);

	PlayerInputComponent->BindAction("BoostDamage",IE_Pressed,this,&AC_PlayerCharacter::StartBoostMastery);
	PlayerInputComponent->BindAction("BoostDamage",IE_Released,this,&AC_PlayerCharacter::EndBoostMastery);
	PlayerInputComponent->BindAction("BoostHealth",IE_Pressed,this,&AC_PlayerCharacter::StartBoostDurability);
	PlayerInputComponent->BindAction("BoostHealth",IE_Released,this,&AC_PlayerCharacter::EndBoostDurability);
	PlayerInputComponent->BindAction("ShowCursor",IE_Pressed,this,&AC_PlayerCharacter::ShowCursor);
	//PlayerInputComponent->BindAction("ShowCursor",IE_Released,this,&AC_PlayerCharacter::ShowCursor);

	PlayerInputComponent->BindAxis("DrinkPotion",this,&AC_PlayerCharacter::UseSlot);
}

void AC_PlayerCharacter::HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload &Payload)
{
	AttackIndex--;
	if(AttackIndex<0 || CharacterData.CurrentEnergy<15 || AttackAnimsIndex==AttackAnims.Num()-1)
	{
		if(GetMesh()->GetAnimInstance())
		{
          GetMesh()->GetAnimInstance()->Montage_Stop(0.3,GetMesh()->GetAnimInstance()->GetCurrentActiveMontage());
		}
		AttackAnimsIndex=0;
	}
	else
	{
     AttackAnimsIndex++;
     PlayAnimMontage(AttackAnims[AttackAnimsIndex],CharacterData.AttackSpeed,NAME_None);

     CharacterData.CurrentEnergy-=12/CharacterData.EnergyMultiplier;
	Cast<AC_PC_PlayerCharacter>(Controller)->PawnEnergyChanged(-0.12f/CharacterData.EnergyMultiplier);

	GetWorldTimerManager().ClearTimer(EnergyCd);
	GetWorldTimerManager().SetTimer(EnergyCd,this,&AC_PlayerCharacter::StartEnergyRegen,1,false);
	}
}

void AC_PlayerCharacter::JoinFight_Implementation()
{
	if(SwordPtr)
	{
		SwordPtr->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,TEXT("SwordSocket2"));
		CurrentStatus=EPlayerStatus::InBattle;
		GetCharacterMovement()->bOrientRotationToMovement=false;
	}
}

void AC_PlayerCharacter::LeaveFight_Implementation()
{
	if(SwordPtr)
	{
		SwordPtr->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,TEXT("SwordSocket1"));
		CurrentStatus=EPlayerStatus::OutOfBattle;
		GetCharacterMovement()->bOrientRotationToMovement=true;
	}
}

void AC_PlayerCharacter::UpStat_Implementation(EStatsToUp stat, float level)
{
	 StartLvlHP=CharacterData.MaxHP/(1+(DurabilityBoostPercent)/100);
	 CharacterData.MaxHP=StartLvlHP;
	 CharacterData.CurrentHP=CharacterData.MaxHP;
	 StartLvlDamage=CharacterData.Damage/(1+MasteryBoostPercent/100);
	 CharacterData.Damage=StartLvlDamage;
	 DurabilityBoostPercent=0;
	 MasteryBoostPercent=0;
	switch (stat)
	{
	case EStatsToUp::AttackSpeed:
		CharacterData.AttackSpeed+=0.075;
		UE_LOG(LogTemp, Display, TEXT("attack speed"));
		break;
	case EStatsToUp::AttackStrength:
	    CharacterData.Damage+=2.5;
		UE_LOG(LogTemp, Display, TEXT("strength"));
		break;
	case EStatsToUp::WalkSpeed:
	    GetCharacterMovement()->MaxWalkSpeed+=20;
		UE_LOG(LogTemp, Display, TEXT("walk speed"));
		break;
	case EStatsToUp::Stamina:
        CharacterData.EnergyMultiplier+=0.25;
		UE_LOG(LogTemp, Display, TEXT("stamina"));
	    break;
	case EStatsToUp::Vampirism:
	    CharacterData.Vampirism+=1.5;
		UE_LOG(LogTemp, Display, TEXT("vampirism"));
		break;
	default:
	UE_LOG(LogTemp, Display, TEXT("other"));
		break;
	}
	StartLvlDamage=CharacterData.Damage;
	Cast<AC_PC_PlayerCharacter>(Controller)->PawnHealthChanged(CharacterData.CurrentHP/CharacterData.MaxHP);
	Cast<AC_PC_PlayerCharacter>(Controller)->PawnDrainingUpdate(CharacterData.Damage,CharacterData.CurrentHP,CharacterData.MaxHP);
}

AActor *AC_PlayerCharacter::GetDamageType_Implementation()
{
	if(DamageType)
	{
		return GetWorld()->SpawnActor<AActor>(DamageType);
	}
    return nullptr;
}

void AC_PlayerCharacter::DrinkPotion_Implementation(EPotionTypes type)
{
	FTimerHandle InfinityEnergy;
	FTimerHandle AdditionalVamp;
	FTimerHandle AttackTimer;
	float StartVamp = CharacterData.Vampirism;
	float StartMulti = CharacterData.EnergyMultiplier;
	float StartAttack = CharacterData.AttackSpeed;
	UE_LOG(LogTemp, Display, TEXT("Your start wave damage%f and hp %f"),StartLvlDamage,StartLvlHP);
	switch (type)
	{
	case EPotionTypes::HealthPotion:
        HealthChange(25);
		UE_LOG(LogTemp, Display, TEXT("drunk health potion"));
		break;
	case EPotionTypes::DamagePotion:
        UE_LOG(LogTemp, Display, TEXT("drunk damage potion"));
		CharacterData.AttackSpeed+=0.4;
		UE_LOG(LogTemp, Display, TEXT("drunk energy potion"));
		GetWorldTimerManager().SetTimer(AttackTimer,[this, StartAttack](){CharacterData.AttackSpeed=StartAttack;},2,false);
		break;
	case EPotionTypes::EnergyPotion:
        CharacterData.EnergyMultiplier+=1;
		UE_LOG(LogTemp, Display, TEXT("drunk energy potion"));
		GetWorldTimerManager().SetTimer(InfinityEnergy,[this, StartMulti](){CharacterData.EnergyMultiplier=StartMulti;},2,false);
		break;
	case EPotionTypes::VampiricPotion:
        CharacterData.Vampirism+=5;
		UE_LOG(LogTemp, Display, TEXT("drunk vampiric potion"));
		GetWorldTimerManager().SetTimer(AdditionalVamp,[this, StartVamp](){CharacterData.Vampirism=StartVamp;},2,false);
	    break;
	default:
	UE_LOG(LogTemp, Display, TEXT("drunk other"));
		break;
	}
	if (DrinkSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, DrinkSound, GetActorLocation());
    }
	Cast<AC_PC_PlayerCharacter>(Controller)->PawnHealthChanged(CharacterData.CurrentHP/CharacterData.MaxHP);
	Cast<AC_PC_PlayerCharacter>(Controller)->PawnDrainingUpdate(CharacterData.Damage,CharacterData.CurrentHP,CharacterData.MaxHP);
}
