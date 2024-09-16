// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Char_EnemyBase.h"
#include "Char_Catcher.generated.h"

UCLASS()
class LONELYSWORD_API AChar_Catcher :  public AChar_EnemyBase
{
	GENERATED_BODY()

private:

UPROPERTY(EditAnywhere, Category = "Animations")
TArray<class UAnimMontage*> HoldingMontage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
    // Sets default values for this character's properties
	AChar_Catcher();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    

	virtual void HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
    
	UFUNCTION()
	void Catched();
    ACharacter* CatchedChar;
	UFUNCTION()
	void Releaf();
	struct FTimerHandle ReleafTimer;

	//Interfaces functions:
	virtual void EnemyPlayMontage_Implementation(class UAnimMontage* MyAnimMontage){};
    virtual void EnemyAttack_Implementation();
};
