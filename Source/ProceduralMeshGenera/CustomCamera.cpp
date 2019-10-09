// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCamera.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"

// Sets default values
ACustomCamera::ACustomCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	player = CreateDefaultSubobject<USceneComponent>(TEXT("Player"));
	RootComponent = player;
	player->SetWorldLocation(FVector(0,0,0));

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	camera->SetupAttachment(RootComponent);
	camera->SetRelativeLocation(FVector(-660,615,340));
	camera->SetRelativeRotation(FRotator(0, 0, 0));

	//Distance between rooms
	distance = 500;
	maxforRow = 3;
}

// Called when the game starts or when spawned
void ACustomCamera::BeginPlay()
{
	Super::BeginPlay();
}

void ACustomCamera::SpawnRoom(UWorld* world, FVector position) {
	ARoom* room = world->SpawnActor<ARoom>();
	room->SetActorLocation(position);
	FString name = FString("Room: " + FString::FromInt(rooms.Num()+1));
	room->SetActorLabel(*name);
	rooms.Add(room);
	selectedRoom = room;
}

void ACustomCamera::MoveForward(float value)
{
	speed.X = value;
}
void ACustomCamera::MoveRight(float value)
{
	speed.Y = value;
}
void ACustomCamera::MoveUp(float value)
{
	speed.Z = value;
}

void ACustomCamera::RotateUp(float value)
{
	rotation.X = value;
}
void ACustomCamera::RotateRight(float value)
{
	rotation.Y = value;
}

void ACustomCamera::AddRoom()
{
	if (deactiveRooms.Num() == 0)
	{
		FVector pos = FVector(x*distance, y*distance, z * distance / 2);
		y++;
		if (y >= maxforRow)
		{
			y = 0;
			x++;
			if (x >= maxforRow)
			{
				x = 0;
				z++;
			}
		}
		SpawnRoom(GetWorld(), pos);
	}
	else {
		deactiveRooms[0]->Deactive(false);
		deactiveRooms.RemoveAt(0);
	}
}
void ACustomCamera::RemoveRoom()
{
	if (selectedRoom != NULL) {
		deactiveRooms.Add(selectedRoom);
		selectedRoom->Deactive(true);
		selectedRoom = NULL;
	}
}

void ACustomCamera::ScaleRoom()
{
	float scaleValue = 1.3f;
	FVector currentScale = selectedRoom->GetActorScale3D();
	FVector newScale = FVector(	currentScale.X * scaleValue, currentScale.Y * scaleValue,currentScale.Z);
	selectedRoom->SetActorScale3D(newScale);
}

void ACustomCamera::SelectRoom()
{
	UE_LOG(LogTemp, Warning, TEXT("Working"));
	FHitResult* hit = new FHitResult();
	FVector startingPoint = GetActorLocation();	
	FVector forward = GetActorForwardVector();
	FVector endPoint = startingPoint + forward * 6000;
	DrawDebugLine(GetWorld(), startingPoint, endPoint, FColor::Red);

	if (GetWorld()->LineTraceSingleByChannel(*hit,startingPoint,endPoint,ECC_Visibility))
	{
		UE_LOG(LogTemp,Warning,TEXT("Name: %s"),*hit->GetActor()->GetName());
	}
}

// Called every frame
void ACustomCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Move camera
	FVector newPosition = GetActorLocation() + speed * DeltaTime * 500;
	SetActorLocation(newPosition);

	//Rotate Camera
	FRotator newRotation = GetActorRotation().Add(rotation.X,rotation.Y,0);
	SetActorRotation(newRotation);

}

// Called to bind functionality to input
void ACustomCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Movement in all 6 directions
	InputComponent->BindAxis("Forward/Backward", this, &ACustomCamera::MoveForward);
	InputComponent->BindAxis("Right/Left", this, &ACustomCamera::MoveRight);
	InputComponent->BindAxis("Up/Down", this, &ACustomCamera::MoveUp);

	//Rotation
	InputComponent->BindAxis("RotateUp/Down", this, &ACustomCamera::RotateUp);
	InputComponent->BindAxis("RotateRight/Left", this, &ACustomCamera::RotateRight);

	//Add Remove selected room
	InputComponent->BindAction("AddRoom", IE_Pressed, this, &ACustomCamera::AddRoom);
	InputComponent->BindAction("RemoveRoom", IE_Pressed, this, &ACustomCamera::RemoveRoom);

	//Scale selected room
	InputComponent->BindAction("ScaleRoom", IE_Pressed, this, &ACustomCamera::ScaleRoom);

	//Mouse Input
	InputComponent->BindAction("MouseLeftClick", IE_Pressed, this, &ACustomCamera::SelectRoom); 
}

