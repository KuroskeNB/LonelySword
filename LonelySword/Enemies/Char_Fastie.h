// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Char_EnemyBase.h"
#include "Char_Fastie.generated.h"

UCLASS()
class LONELYSWORD_API AChar_Fastie :  public AChar_EnemyBase
{
	GENERATED_BODY()

private:
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
    AChar_Fastie();


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    


	void HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//Interfaces functions:
	virtual void EnemyPlayMontage_Implementation(class UAnimMontage* MyAnimMontage){};
    virtual void EnemyAttack_Implementation();
};
