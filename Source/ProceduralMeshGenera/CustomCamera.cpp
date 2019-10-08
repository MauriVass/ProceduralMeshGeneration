// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCamera.h"
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
	camera->SetRelativeLocation(FVector(-250,0,250));
	camera->SetRelativeRotation(FRotator(-45, 0, 0));

}

// Called when the game starts or when spawned
void ACustomCamera::BeginPlay()
{
	Super::BeginPlay();

	//SpawnRoom(GetWorld(),FVector(0,0,0));
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

void ACustomCamera::AddRoom()
{
	SpawnRoom(GetWorld(), FVector(90));
}
void ACustomCamera::RemoveRoom()
{
	UE_LOG(LogTemp, Warning, TEXT("Hidding %s"), *selectedRoom->GetName());
	selectedRoom->SetActorHiddenInGame(true);
}

void ACustomCamera::ScaleRoom()
{
	float scaleValue = 1.3f;
	FVector currentScale = selectedRoom->GetActorScale3D();
	FVector newScale = FVector(	currentScale.X * scaleValue, currentScale.Y * scaleValue,currentScale.Z);
	selectedRoom->SetActorScale3D(newScale);
}

// Called every frame
void ACustomCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//selectedRoom = rooms[0];

	FVector newPosition = GetActorLocation() + speed * DeltaTime * 500;
	SetActorLocation(newPosition);

}

// Called to bind functionality to input
void ACustomCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Movement in all 6 directions
	InputComponent->BindAxis("Forward/Backward", this, &ACustomCamera::MoveForward);
	InputComponent->BindAxis("Right/Left", this, &ACustomCamera::MoveRight);
	InputComponent->BindAxis("Up/Down", this, &ACustomCamera::MoveUp);

	//Add Remove selected room
	InputComponent->BindAction("AddRoom", IE_Pressed, this, &ACustomCamera::AddRoom);
	InputComponent->BindAction("RemoveRoom", IE_Pressed, this, &ACustomCamera::RemoveRoom);

	InputComponent->BindAction("ScaleRoom", IE_Pressed, this, &ACustomCamera::ScaleRoom);
}

