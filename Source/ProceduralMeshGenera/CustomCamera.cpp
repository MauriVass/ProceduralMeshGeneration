// Custom 'player'. It can move in all directions, rotate the view, spawn new table, change their size and delete them


#include "CustomCamera.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"
//For mouse input
#include "GameFramework/PlayerController.h"

// Sets default values
ACustomCamera::ACustomCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	player = CreateDefaultSubobject<USceneComponent>(TEXT("Player"));
	RootComponent = player;
	player->SetWorldLocation(FVector(-1100, 610, 290));

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	camera->SetupAttachment(player);
	camera->SetRelativeRotation(FRotator(0, 0, 0));

	indexSelectedRoom = -1;

	//Distance between rooms
	distance = 1000;
	//Max number of rooms for row
	maxforRow = 2;
}

// Called when the game starts or when spawned
void ACustomCamera::BeginPlay()
{
	Super::BeginPlay();
}

void ACustomCamera::SpawnRoom(UWorld* world, FVector position) {
	//Instantiate a room actor
	ARoom* room = world->SpawnActor<ARoom>();
	//Set the position
	room->SetActorLocation(position);
	//Set the name of the new room
	FString name = FString("Room: " + FString::FromInt(activeRooms.Num()+1));
	room->SetActorLabel(*name);
	//Add the new room to the rooms array
	activeRooms.Add(room);
}

//Move the camera with ASDW and UO for up/down
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

//Rotate the camera with JKLI
void ACustomCamera::RotateUp(float value)
{
	rotation.X = value;
}
void ACustomCamera::RotateRight(float value)
{
	rotation.Y = value;
}

//Add room and set the position 
void ACustomCamera::AddRoom()
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
//Remove the selected room
void ACustomCamera::RemoveRoom()
{
	if (selectedRoom != NULL) {
		activeRooms.Remove(selectedRoom);
		indexSelectedRoom--;
		selectedRoom->Delete(true);
		selectedRoom = NULL;
	}
}

//Scale the selected room with B/N, left mouse
void ACustomCamera::ScaleRoom(float value) {
	if (selectedRoom != NULL)
	{
		selectedRoom->GetTable()->GetSurface()->UpdateVertice(indexPivot, value);
		selectedRoom->GetTable()->SetObjectsPosition(indexPivot, value);
		selectedRoom->SetChairsPosition(indexPivot, value);
	}
}
//Scale up with Z, scale down with X 
void ACustomCamera::ScaleRoomNotDiscrete(float value)
{
	float v = 1;
	if (selectedRoom != NULL)
	{
		if (value < 0 && selectedRoom->GetTable()->GetSurfaceWidth() > 3.1f * selectedRoom->GetChairWidth())
		{
			/*selectedRoom->GetTable()->GetSurface()->UpdateVertice(indexPivot, value);
			selectedRoom->GetTable()->SetObjectsPosition(indexPivot, value);
			selectedRoom->SetChairsPosition(indexPivot, value);*/
			ScaleRoom(value);
			selectedRoom->RemoveChairs();
		}
		else if(value > 0) {
			/*selectedRoom->GetTable()->GetSurface()->UpdateVertice(indexPivot, value);
			selectedRoom->GetTable()->SetObjectsPosition(indexPivot, value);
			selectedRoom->SetChairsPosition(indexPivot, value);*/
			ScaleRoom(value);
			selectedRoom->AddChairs(indexPivot);
		}
	}
}
void ACustomCamera::ScaleRoomUp()
{
	if (selectedRoom != NULL)
	{
		ScaleRoom(20);
		selectedRoom->AddChairs(indexPivot);
	}
}
void ACustomCamera::ScaleRoomDown()
{
	//This if avoid to make the table too small
	if (selectedRoom->GetTable()->GetSurfaceWidth() > 3.1f * selectedRoom->GetChairWidth())
	{
		ScaleRoom(-20);
		selectedRoom->RemoveChairs();
	}
}

void ACustomCamera::MouseDown()
{
	isMouseDown = true;
	Cast<APlayerController>(GetController())->GetMousePosition(initialMousePos.X,initialMousePos.Y);
}
void ACustomCamera::MouseUp()
{
	isMouseDown = false;
}

//Select with RIGHT and LEFT ARROW
void ACustomCamera::SelectRoom(bool up)
{
	if (activeRooms.Num() != 0)
	{
		//Set pivots animation of the current room as false so they will not move anymore
		if (indexSelectedRoom>=0 && indexSelectedRoom<activeRooms.Num())
			activeRooms[indexSelectedRoom]->GetTable()->GetCentralPivot()->Animate(false);
		if (selectedRoom != NULL)
		{
			for (int32 i = 0; i < 4; i++)
			{
				selectedRoom->GetTable()->GetPivot(i)->Animate(false);
			}
		}
		//If Up ARROW is pressed increase counter
		//otherwise decrease it
		if (up)
		{
			indexSelectedRoom++;
			if (indexSelectedRoom > activeRooms.Num() - 1)
				indexSelectedRoom = 0;
		}
		else
		{
			indexSelectedRoom--;
			if (indexSelectedRoom < 0)
				indexSelectedRoom = activeRooms.Num() - 1;
		}
		//changeRoom = true;
		//Set animation for the central pivot as true so it will start moving
		if (indexSelectedRoom >= 0 && indexSelectedRoom < activeRooms.Num())
			activeRooms[indexSelectedRoom]->GetTable()->GetCentralPivot()->Animate(true);
		//
		selectedRoom = activeRooms[indexSelectedRoom];
		selectedRoom->GetTable()->GetPivot(indexPivot)->Animate(true);
	}
}
void ACustomCamera::SelectRoomUp()
{
	SelectRoom(true);
}
void ACustomCamera::SelectRoomDown()
{
	SelectRoom(false);
}

//Select with UP and DOWN ARROW
void ACustomCamera::SelectPivot(bool up)
{
	if (selectedRoom != NULL)
	{
		//Set current pivot animation as false
		selectedRoom->GetTable()->GetPivot(indexPivot)->Animate(false);
		//Change pivot according with key pressed
		if (up)
		{
			indexPivot++;
			if (indexPivot > 4 - 1)
				indexPivot = 0;
		}
		else {
			indexPivot--;
			if (indexPivot < 0)
				indexPivot = 4 - 1;
		}
		//Set new current pivot animation as true
		selectedRoom->GetTable()->GetPivot(indexPivot)->Animate(true);
	}
}
void ACustomCamera::SelectPivotUp()
{
	SelectPivot(true);
}
void ACustomCamera::SelectPivotDown()
{
	SelectPivot(false);
}

// Called every frame
void ACustomCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Move camera
	FVector newPosition = GetActorLocation() + speed * DeltaTime * 750;
	SetActorLocation(newPosition);

	//Rotate Camera
	FRotator newRotation = GetActorRotation().Add(rotation.X,rotation.Y,0);
	SetActorRotation(newRotation);

	if (changeRoom)
	{
		changeRoom = false;
	}
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

	//Room selection
	InputComponent->BindAction("SelectRoomUp", IE_Pressed, this, &ACustomCamera::SelectRoomUp);
	InputComponent->BindAction("SelectRoomDown", IE_Pressed, this, &ACustomCamera::SelectRoomDown);

	//Pivot selection
	InputComponent->BindAction("SelectPivotUp", IE_Pressed, this, &ACustomCamera::SelectPivotUp);
	InputComponent->BindAction("SelectPivotDown", IE_Pressed, this, &ACustomCamera::SelectPivotDown);

	//Scale selected room with B and N (descrete)
	InputComponent->BindAction("ScaleRoomUp", IE_Pressed, this, &ACustomCamera::ScaleRoomUp);
	InputComponent->BindAction("ScaleRoomDown", IE_Pressed, this, &ACustomCamera::ScaleRoomDown);
	//Scale selected room with mouse
	InputComponent->BindAction("MouseLeftClick", IE_Pressed, this, &ACustomCamera::MouseDown);
	InputComponent->BindAction("MouseLeftClick", IE_Released, this, &ACustomCamera::MouseUp);
	//Scale selected room with Z and X (not discrete)
	InputComponent->BindAxis("Scale", this, &ACustomCamera::ScaleRoomNotDiscrete);

	if (isMouseDown)
	{
		FVector newPos;
		Cast<APlayerController>(GetController())->GetMousePosition(newPos.X, newPos.Y);
		scaleValue = FMath::Sqrt(FMath::Sqrt(initialMousePos.X - newPos.X) + FMath::Sqrt(initialMousePos.Y - newPos.Y));
		scaleValue = newPos.Y > initialMousePos.Y ? scaleValue : -scaleValue;
		ScaleRoom(scaleValue);
	}

	
}

