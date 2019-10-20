// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Room.h"
#include "Camera/CameraComponent.h"
#include "CustomCamera.generated.h"

UCLASS()
class PROCEDURALMESHGENERA_API ACustomCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACustomCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	USceneComponent* player;
	UCameraComponent* camera;
	APlayerController* pc;

	TArray<ARoom*> activeRooms;
	int32 roomCounter;
	ARoom* selectedRoom;
	int32 indexSelectedRoom;
	ABoxModel* selectedPivot;
	
	void SpawnRoom(UWorld * world, FVector position);

	FVector speed;
	void MoveForward(float value);
	void MoveRight(float value);
	void MoveUp(float value);

	FVector rotation;
	void RotateUp(float value);
	void RotateRight(float value);

	void AddRoom();
	void RemoveRoom();

	void SelectRoom(int cases);
	void SelectRoomUp();
	void SelectRoomDown();

	int32 indexPivot;
	void SelectPivot(bool up);
	void SelectPivotUp();
	void SelectPivotDown();

	void ScaleRoom(float value);
	void ScaleRoomNotDiscrete(float value);
	void ScaleRoomUp();
	void ScaleRoomDown();
	float scaleValue;
	bool isMouseDown, canScale;
	FVector initialMousePos;
	void MouseDown();
	void MouseUp();

	int32 x, y, z, distance, maxforRow;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
