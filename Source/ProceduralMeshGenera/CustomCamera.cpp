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
	
	//This is -1 so that any room is selected at the beginning
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

	//Create a player controller to enable mouse input
	pc = Cast<APlayerController>(GetController());
	pc->bShowMouseCursor = true;
	pc->bEnableClickEvents = true;
	pc->bEnableMouseOverEvents = true;
}

void ACustomCamera::SpawnRoom(UWorld* world, FVector position) {
	//Instantiate a room actor
	ARoom* room = world->SpawnActor<ARoom>();
	//Set the position
	room->SetActorLocation(position);
	//Set room id
	room->SetID(roomCounter++);
#if WITH_EDITOR
	//Set the name of the new room
	FString name = FString("Room: " + FString::FromInt(activeRooms.Num()+1));
	room->SetActorLabel(*name);
#endif
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

void ACustomCamera::ScaleRoom(float value) {
	if (selectedRoom != NULL)
	{
		selectedRoom->GetTable()->GetSurface()->UpdateVertice(indexPivot, value);
		selectedRoom->GetTable()->SetObjectsPosition(indexPivot, value);
		selectedRoom->SetChairsPosition(indexPivot, value);
	}
}
//Scale up with Z, scale down with X or left mouse
void ACustomCamera::ScaleRoomNotDiscrete(float value)
{
	if (selectedRoom != NULL)
	{
		if (value < 0 && selectedRoom->GetTable()->GetSurfaceWidth() + value > 3.1f * selectedRoom->GetChairWidth())
		{
			ScaleRoom(value);
			selectedRoom->RemoveChairs();
		}
		else if(value > 0) {
			ScaleRoom(value);
			selectedRoom->AddChairs(indexPivot);
		}
	}
}
//Scale the selected room with B/N
void ACustomCamera::ScaleRoomUp()
{
	if (selectedRoom != NULL)
	{
		ScaleRoom(10);
		selectedRoom->AddChairs(indexPivot);
	}
}
void ACustomCamera::ScaleRoomDown()
{
	int32 size = -10;
	//This if avoids to make the table too small
	if (selectedRoom->GetTable()->GetSurfaceWidth() + size > 3.1f * selectedRoom->GetChairWidth())
	{
		ScaleRoom(size);
		selectedRoom->RemoveChairs();
	}
}

void ACustomCamera::MouseDown()
{
	//isMouseDown is use to trace line, when you hit something isMouseDown becomes false 
	//so that you can scale without selecting other objects using the same button
	//canScale is used to scale
	isMouseDown = true;
	canScale = true;
	pc->GetMousePosition(initialMousePos.X, initialMousePos.Y);
}
void ACustomCamera::MouseUp()
{
	isMouseDown = false;
	canScale = false;
}

//Select with RIGHT and LEFT ARROW
void ACustomCamera::SelectRoom(int cases)
{
	if (activeRooms.Num() != 0)
	{
		//Set pivots animation of the current room as false so they will not move anymore
		if (selectedRoom!=NULL)
			selectedRoom->GetTable()->GetCentralPivot()->Animate(false);
		if (selectedRoom != NULL)
		{
			for (int32 i = 0; i < 4; i++)
			{
				selectedRoom->GetTable()->GetPivot(i)->Animate(false);
			}
		}
		//If Up ARROW is pressed increase counter
		//otherwise decrease it (case 0)
		if (cases==0)
		{
			indexSelectedRoom++;
			if (indexSelectedRoom > activeRooms.Num() - 1)
				indexSelectedRoom = 0;
		}
		else if(cases == 1) //Down Arrow
		{
			indexSelectedRoom--;
			if (indexSelectedRoom < 0)
				indexSelectedRoom = activeRooms.Num() - 1;
		}
		if (cases != 2) {//Mouse selection

			//Set animation for the central pivot as true so it will start moving
			if (indexSelectedRoom >= 0 && indexSelectedRoom < activeRooms.Num())
				activeRooms[indexSelectedRoom]->GetTable()->GetCentralPivot()->Animate(true);

			selectedRoom = activeRooms[indexSelectedRoom];
			selectedPivot = selectedRoom->GetTable()->GetPivot(indexPivot);
			selectedPivot->Animate(true);
		}
	}
}
void ACustomCamera::SelectRoomUp()
{
	SelectRoom(0);
}
void ACustomCamera::SelectRoomDown()
{
	SelectRoom(1);
}

//Select with UP and DOWN ARROW
void ACustomCamera::SelectPivot(bool up)
{
	if (selectedRoom != NULL)
	{
		//Set current pivot animation as false
		selectedPivot->Animate(false);
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
		selectedPivot = selectedRoom->GetTable()->GetPivot(indexPivot);
		//Set new current pivot animation as true
		selectedPivot->Animate(true);
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
	FVector newPosition = GetActorLocation() + //Actuall Location
		( GetActorForwardVector() * speed.X + //Forward Direction
		FVector::CrossProduct(GetActorUpVector(),GetActorForwardVector()) * speed.Y + //Side Direction
		GetActorUpVector() * speed.Z //Up Direction
			) * DeltaTime * 750; //Multiplier
	//Set new location
	SetActorLocation(newPosition);

	//Rotate Camera
	FRotator newRotation = GetActorRotation().Add(rotation.X,rotation.Y,0);
	SetActorRotation(newRotation);

	if (isMouseDown)
	{
		FHitResult  OutHit;
		FVector Start = GetActorLocation();
		FVector ForwardVector = GetActorForwardVector();
		pc->DeprojectMousePositionToWorld(Start, ForwardVector);

		//Shoot a line to select room or pivot.
		//When left mouse button is clicked a line is throw, there are some cases:
		//First of all the object hit must be an instance of class ABoxModel, then
		//1)selectedRoom is NULL, it means the there is no room seleted so select the room of the hit object
		//2)selectedRoom is not NULL, the object hit is and edge pivot and this pivot belong to the selected room, the select the pivot hit
		//3)selectedRoom is not NULL, the object hit does not belong to the current selected room, then change the selectedRoom with the object hit
		if (GetWorld()->LineTraceSingleByChannel(OutHit, ForwardVector * 30 + Start, ForwardVector * 3000 + Start, ECC_Visibility, FCollisionQueryParams()))
		{
			if (OutHit.GetActor()->GetClass()->IsChildOf(ABoxModel::StaticClass()))
			{
				ABoxModel* tmp = Cast<ABoxModel>(OutHit.GetActor());
				if (selectedRoom!=NULL)
				{
					//The selected room is the same of the room which current object clicked belongs and
					//The object clicked is an edgePivot (no central one)
					if (selectedRoom->GetID() == Cast<ARoom>(tmp->GetRoom())->GetID() && tmp->GetIsEdgePivot())
					{
						/**Case 2**/
						//Set current pivot animation as false
						selectedPivot->Animate(false);
						selectedPivot = tmp;
						selectedRoom = Cast<ARoom>(tmp->GetRoom());
						//Set new current pivot animation as true
						selectedPivot->Animate(true);
						indexPivot = selectedRoom->GetTable()->GetIndex(tmp);
					}
					else {
						/**Case 3**/
						SelectRoom(2);
						selectedRoom = Cast<ARoom>(tmp->GetRoom());
						selectedRoom->GetTable()->GetCentralPivot()->Animate(true);
						indexPivot = 0;
						selectedPivot = selectedRoom->GetTable()->GetPivot(indexPivot);
						selectedPivot->Animate(true);
					}
				}
				else {
					/**Case 1**/
					SelectRoom(2);
					selectedRoom = Cast<ARoom>(tmp->GetRoom());
					selectedRoom->GetTable()->GetCentralPivot()->Animate(true);
					indexPivot = 0;
					selectedPivot = selectedRoom->GetTable()->GetPivot(indexPivot);
					selectedPivot->Animate(true);
				}
			}
			isMouseDown = false;
		}
	}

		if (selectedRoom != NULL && canScale)
		{
			FVector newPos;
			pc->GetMousePosition(newPos.X, newPos.Y);

			//Square root distance
			scaleValue = FMath::Sqrt(FMath::Square(initialMousePos.X - newPos.X) + FMath::Square(initialMousePos.Y - newPos.Y));
			//Or linear distance
			//scaleValue = FMath::Abs(initialMousePos.Y - newPos.Y);

			//Scale according with the position of the camera
			if (indexPivot > 1)
				scaleValue = newPos.Y > initialMousePos.Y ? -scaleValue : scaleValue;
			else 
				scaleValue = newPos.Y > initialMousePos.Y ? scaleValue : -scaleValue;

			ScaleRoomNotDiscrete(scaleValue / 100);
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
	   	
}

