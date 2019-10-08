// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxModel.h"
#include "Chair.generated.h"

UCLASS()
class PROCEDURALMESHGENERA_API AChair : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChair();
	int32 GetSurfaceWidth();
	int32 GetSpacing();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UStaticMeshComponent* chair;
	int32 surfaceWidth, spacing;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
