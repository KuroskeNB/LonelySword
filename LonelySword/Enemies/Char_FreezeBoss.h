// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Char_EnemyBase.h"
#include "Char_FreezeBoss.generated.h"

/**
 * 
 */
UCLASS()
class LONELYSWORD_API AChar_FreezeBoss : public AChar_EnemyBase
{
	GENERATED_BODY()
	public:
	// Sets default values for this character's properties
	AChar_FreezeBoss();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
 
	//Interfaces functions:
	virtual void EnemyPlayMontage_Implementation(class UAnimMontage* MyAnimMontage){};
    virtual void EnemyAttack_Implementation() override;
	virtual void HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload) override;
	void Ultimate(FName BoneName);
	void FootAttack(FName BoneName);
	void FistCombo(FName BoneName);

	void FreezeNotify(AActor* Actor);
	bool bFreezed=false;
	int FreezeStacks=0;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
    
	private:
	UPROPERTY(EditAnywhere, Category = "Animations")
    class UAnimMontage* UltimateMontage;
	UPROPERTY()
	AActor* TargetForUlt;
	UPROPERTY(EditAnywhere, Category = "Particles")
    class UParticleSystem* UltimateParticle;
};
