// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Char_EnemyBase.h"
#include "Char_PoisonBoss.generated.h"

/**
 * 
 */
UCLASS()
class LONELYSWORD_API AChar_PoisonBoss : public AChar_EnemyBase
{
	GENERATED_BODY()
	public:
	// Sets default values for this character's properties
	AChar_PoisonBoss();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
 
	//Interfaces functions:
	virtual void EnemyPlayMontage_Implementation(class UAnimMontage* MyAnimMontage){};
    virtual void EnemyAttack_Implementation() override;
    void CloudAttack(FName BoneName);
	void AreaAttack(FName BoneName);
	void Ultimate(FName BoneName);
	virtual void HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
    UPROPERTY()
	float AttackIndex=0;
    
	void PoisonNotify(AActor* Actor);
	float PoisonStacks=0;
	private:
	UPROPERTY(EditAnywhere, Category = "Particles")
    class UParticleSystem* CloudParticle;
	UPROPERTY(EditAnywhere, Category = "Particles")
    class UParticleSystem* AreaParticle;
	UPROPERTY(EditAnywhere, Category = "Animations")
    class UAnimMontage* UltimateMontage;
	UPROPERTY()
	AActor* TargetForUlt;
	UPROPERTY(EditAnywhere, Category = "Particles")
    class UParticleSystem* UltimateParticle;
};
