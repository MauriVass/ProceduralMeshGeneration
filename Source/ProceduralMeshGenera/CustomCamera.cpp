// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCamera.h"

// Sets default values
ACustomCamera::ACustomCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACustomCamera::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACustomCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsInputKeyDown(EKeys::A))
	{
		
	}

}

// Called to bind functionality to input
void ACustomCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

