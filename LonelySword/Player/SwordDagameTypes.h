// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwordDagameTypes.generated.h"


UENUM(BlueprintType)
enum class EDamageType : uint8    // (uint8, uint16)
{
	Impulse UMETA(DisplayName = "Impulse"),
	Poison UMETA(DisplayName = "Poison"),
	Freeze UMETA(DisplayName = "Freeze"),
	Splash UMETA(DisplayName = "Splash")
};
/**
 * 
 */
UCLASS()
class LONELYSWORD_API ASwordDagameTypes : public AActor
{
	GENERATED_BODY()
	public:
    UFUNCTION()
	void ImplementEffectTo(AActor* actor);

	private:
	UPROPERTY(EditAnywhere)
	EDamageType DamageType;

	int PoisonIndex = 0;

	UFUNCTION()
	void Freeze(AActor* actor);
	UFUNCTION()
	void Impulse(AActor* actor);
	UFUNCTION()
	void Splash(AActor* actor);
	UFUNCTION()
	void Poison(AActor* actor);

	public:
	UPROPERTY(EditAnywhere, Category = "Particles")
    class UParticleSystem* TypeParticle;
	UPROPERTY(EditAnywhere,Category="Types Data")
	float FreezeDuration=1.2;
	UPROPERTY(EditAnywhere,Category="Types Data")
	float ImpulseForce=2500;
	UPROPERTY(EditAnywhere,Category="Types Data")
	float SplashDamage=5;
	UPROPERTY(EditAnywhere,Category="Types Data")
	float SplashRadius=100;
	UPROPERTY(EditAnywhere,Category="Types Data")
	float PoisonDamage=5;
	UPROPERTY(EditAnywhere,Category="Types Data")
	float PoisonInterval=0.8;
};
