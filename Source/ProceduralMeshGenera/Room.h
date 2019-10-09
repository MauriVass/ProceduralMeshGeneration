// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Chair.h"
#include "Table.h"
#include "Room.generated.h"

UCLASS()
class PROCEDURALMESHGENERA_API ARoom : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	ARoom();

	//USceneComponent* room;
	UBoxComponent* room;

	void Deactive(bool value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<AChair*> chairs;
	ATable* table;

	void CreateRoom(UWorld* world, AActor* actorParent);
	void CreateSetofChairs(UWorld* world, AActor* actorParent);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
