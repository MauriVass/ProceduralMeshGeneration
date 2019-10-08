// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxModel.h"
#include "Table.generated.h"

UCLASS()
class PROCEDURALMESHGENERA_API ATable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATable();

	int32 GetSurfaceWidth();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UStaticMeshComponent* table;
	int32 surfaceWidth;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
