// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Char_EnemyBase.h"
#include "Char_ImpulseBoss.generated.h"

/**
 * 
 */
UCLASS()
class LONELYSWORD_API AChar_ImpulseBoss : public AChar_EnemyBase
{
	GENERATED_BODY()
	public:
	// Sets default values for this character's properties
	AChar_ImpulseBoss();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
 
	//Interfaces functions:
	virtual void EnemyPlayMontage_Implementation(class UAnimMontage* MyAnimMontage){};
    virtual void EnemyAttack_Implementation() override;

    void Stomp(FName BoneName);
	void Slam(FName BoneName);
    void Ultimate(FName BoneName);
    void ImpulseNotify(AActor* Actor);
    void HookActor(AActor* Actor);
	virtual void HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
UPROPERTY(EditAnywhere, Category = "Animations")
    class UAnimMontage* UltimateMontage;
	UPROPERTY()
	AActor* TargetForUlt;
	UPROPERTY(EditAnywhere, Category = "Particles")
    class UParticleSystem* UltimateParticle;
	float ImpulseStacks=0;
};
