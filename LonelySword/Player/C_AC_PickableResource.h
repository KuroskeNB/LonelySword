// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_AC_PickableResource.generated.h"

UCLASS()
class LONELYSWORD_API AC_AC_PickableResource : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_AC_PickableResource();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
UPROPERTY(EditAnywhere)
TMap<FName,float> Resources;
UPROPERTY(EditAnywhere)
TObjectPtr<class UStaticMeshComponent> ResourceMesh;
};
