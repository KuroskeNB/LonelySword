// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../LonelySwordCharacter.h"
#include "C_Interface_PlayerCharacter.h"
#include "C_Struct_CharacterData.h"
#include "C_PlayerCharacter.generated.h"


UCLASS()
class LONELYSWORD_API AC_PlayerCharacter : public ALonelySwordCharacter, public IC_Interface_PlayerCharacter
{
	GENERATED_BODY()
private:
UPROPERTY()
struct FTimerHandle EnergyCd;
UFUNCTION()
void StartEnergyRegen();
//Charcter stats
UPROPERTY(EditAnywhere,Category="Character Stats")
FCharacterData CharacterData;
// animations:
UPROPERTY(EditAnywhere, Category = "Animations|Battle")
TArray<class UAnimMontage*> AttackComboAnim;
UPROPERTY(EditAnywhere, Category = "Animations|Battle")
TArray<class UAnimMontage*> AttackAnims;
float AttackAnimsIndex=0;
UFUNCTION()
bool IsAttacking();
float AttackIndex=0;
UPROPERTY(EditAnywhere, Category = "Sound")
USoundBase* ImpactSound;
UPROPERTY(EditAnywhere, Category = "Sound")
USoundBase* SwordSound;
UPROPERTY(EditAnywhere, Category = "Sound")
USoundBase* DrinkSound;
UPROPERTY(EditAnywhere, Category = "Animations")
class UAnimMontage* DrawSwordAnim;
UPROPERTY(EditAnywhere, Category = "Animations")
class UAnimMontage* StrafeAnim;
UPROPERTY(EditAnywhere, Category = "Animations")
class UAnimMontage* SeathSwordAnim;
UPROPERTY(EditAnywhere, Category = "Animations")
class UAnimMontage* JumpAnim;
UPROPERTY(EditAnywhere, Category = "Animations")
TArray<class UAnimMontage*> ImpactMontage;
UPROPERTY(EditAnywhere, Category = "Particles")
class UParticleSystem* ImpactParticle;

UPROPERTY(EditAnywhere)
TSubclassOf<AActor> SwordClass;
UPROPERTY()
TObjectPtr<AActor> SwordPtr;
UPROPERTY()
EPlayerStatus CurrentStatus=EPlayerStatus::OutOfBattle;
UPROPERTY(EditAnywhere)
TSubclassOf<class ASwordDagameTypes> DamageType;
public:
	// Sets default values for this character's properties
	AC_PlayerCharacter();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// Player actions:
    UFUNCTION()
    virtual void SwordAttack();
	UFUNCTION()
	virtual void EndBattle();
    UFUNCTION()
	virtual void Strafe();
	UFUNCTION()
	virtual void OpenInventory();
	UFUNCTION()
	virtual void PickupItem();
	UFUNCTION()
	void UseSlot(float index);
    UFUNCTION()
	void ShowCursor();
	bool bCursorIsShown=false;

	/// hp and damage boosting
    UPROPERTY() 
    struct FTimerHandle DrainingTimer;
	UPROPERTY()
	float PercentMaxCapacity=100;
	float DurabilityBoostPercent=0;
	float MasteryBoostPercent=0;
	UFUNCTION()
    void StartBoostDurability();
	UFUNCTION()
	void EndBoostDurability();
	UFUNCTION()
    void StartBoostMastery();
	UFUNCTION()
	void EndBoostMastery();
	float StartLvlHP=0;
	float StartLvlDamage=0;
	void HealthChange(float change);
	void Death();
	void FullRecover();
	////////////////////
	float previousindex=0;
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
    UFUNCTION()
    void OnSwordOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void CheckDamageForEffects(AActor* DamageCauser);
	UFUNCTION()
	void SaveLevel();
	// End of Player actions
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
	//Battle system
	UFUNCTION()
	void HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload);
	//Interface inherit functions:
    virtual void JoinFight_Implementation();
    virtual void LeaveFight_Implementation();
	EPlayerStatus GetPlayerStatus_Implementation(){return CurrentStatus;};
	virtual void UpStat_Implementation(EStatsToUp stat, float level);
	virtual AActor* GetDamageType_Implementation();
	virtual void DrinkPotion_Implementation(EPotionTypes type);
};
