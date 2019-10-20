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

	ABoxModel* GetSurface();

	float GetSurfaceWidth();
	ABoxModel* GetCentralPivot();
	ABoxModel* GetPivot(int32 index);
	int32 GetIndex(ABoxModel* pivot);
	void SetObjectsPosition(int32 pivot, float value);
	void Delete();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USceneComponent* table;

	int32 legWidth,legHeight;
	//This is a float value since scaling with mouse to some conversion (from float to int) problems
	float surfaceWidth;
	int32 surfaceHeight;

	int32 pivotWidth;
	float offset;

	TArray<ABoxModel*> legs;
	ABoxModel* surface;
	ABoxModel* centralPivot;
	TArray<ABoxModel*> pivots;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
