// Fill out your copyright notice in the Description page of Project Settings.


#include "C_AC_PickableResource.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AC_AC_PickableResource::AC_AC_PickableResource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
	ResourceMesh=CreateDefaultSubobject<UStaticMeshComponent>("Resource Mesh");
	ResourceMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AC_AC_PickableResource::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_AC_PickableResource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

