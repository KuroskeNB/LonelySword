// Fill out your copyright notice in the Description page of Project Settings.


#include "C_WeaponSword.h"
#include "Components/SkeletalMeshComponent.h"

void AC_WeaponSword::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
}

// Sets default values
AC_WeaponSword::AC_WeaponSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
	SwordMesh=CreateDefaultSubobject<USkeletalMeshComponent>("Sword Mesh");
}

// Called when the game starts or when spawned
void AC_WeaponSword::BeginPlay()
{
	Super::BeginPlay();
	
	if(SwordMesh)
	{
		SwordMesh->OnComponentBeginOverlap.AddDynamic(this,&AC_WeaponSword::OnOverlapBegin);
	}
}

// Called every frame
void AC_WeaponSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

