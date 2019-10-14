// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "BoxModel.generated.h"

UCLASS()
class PROCEDURALMESHGENERA_API ABoxModel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoxModel();

	void GenerateBox(int32 width, int32 height, bool leg = true);

	static ABoxModel* CreatePiece(UWorld* world, FVector position, FRotator rotation, int32 width, int32 height, bool isLeg, FString name, AActor* actorParent);
	
	void TranslateInitialPosition(FVector pos);
	void SetShouldAnimate(bool value);
	void Animate(bool value);

	void UpdateVertice(int32 index, float value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent* box;

	TArray<FVector> vertices;

	TArray<int32> triangles;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* material;

	void AddVertex(FVector position);

	void AddTriangle(int32 v1, int32 v2, int32 v3);

	bool enableAnimation, shouldAnimate;
	FVector initialPosition;
	float time;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
