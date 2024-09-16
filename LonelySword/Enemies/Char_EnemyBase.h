// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_Interface_EnemyCharacter.h"
#include "../Player/C_Struct_CharacterData.h"
#include "Char_EnemyBase.generated.h"

UCLASS()
class LONELYSWORD_API AChar_EnemyBase : public ACharacter, public IC_Interface_EnemyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AChar_EnemyBase();
    
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
	//Interfaces functions:
	virtual void EnemyPlayMontage_Implementation(class UAnimMontage* MyAnimMontage){};
	virtual void CheckDamageForEffects_Implementation(AActor* DamageCauser);
    virtual void DropItems_Implementation();
	virtual bool IsAlive_Implementation();
	virtual AActor* GetDamageType_Implementation();
	void SetLevelOfPower_Implementation(int level);
	float GetPawnAttackDistance_Implementation(){return CharacterData.AttackDistance;};
	virtual bool IsAbleToMove_Implementation(){return bIsAbleToMove;};
	bool bIsAbleToMove=true;

	//
	UFUNCTION()
	virtual void Die(float level);
	UFUNCTION()
	virtual bool IsAttacking();
    UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION()
	virtual void HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

UPROPERTY(EditAnywhere, Category = "Animations")
TArray<class UAnimMontage*> AttackMontage;
UPROPERTY(EditAnywhere, Category = "Animations")
class UAnimMontage* DeathMontage;
UPROPERTY(EditAnywhere, Category = "Animations")
class UAnimMontage* DeathState;
UPROPERTY(EditAnywhere, Category = "Animations")
TArray<class UAnimMontage*> ImpactMontage;
bool bCanPunch =true;
UPROPERTY(EditAnywhere, Category = "Particles")
class UParticleSystem* ImpactParticle;
UPROPERTY(EditAnywhere, Category = "Particles")
class UParticleSystem* AttackParticle;

UPROPERTY(EditAnywhere, Category = "Drop")
TSubclassOf<class AC_AC_PickableResource> DropItemClass;
UPROPERTY(EditAnywhere, Category = "Drop")
float DropItemsCount=1;

UPROPERTY(EditAnywhere,Category="Character Stats")
FCharacterData CharacterData;
bool bIsAlive=true;

UPROPERTY(EditAnywhere)
TSubclassOf<class ASwordDagameTypes> DamageType;
UPROPERTY(EditAnywhere, Category = "UI")
class UWidgetComponent* HealthBarComponent;
};
