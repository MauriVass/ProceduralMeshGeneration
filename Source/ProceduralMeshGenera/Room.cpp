//Collection of table and chairs

#include "Room.h"

// Sets default values
ARoom::ARoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	room = CreateDefaultSubobject<USceneComponent>(TEXT("Room"));
	RootComponent = room;

	chairs = TArray<AChair*>();
}

void ARoom::SetID(int32 id)
{
	ARoom::id = id;
}

int32 ARoom::GetID()
{
	return id;
}

// Called when the game starts or when spawned
void ARoom::BeginPlay()
{
	Super::BeginPlay();
	
	CreateRoom(GetWorld(), this);
}

void ARoom::CreateRoom(UWorld* world, AActor* actorParent) {
	//Store memory for one leg table shape in the position and rotation set in the constructor
	table = world->SpawnActor<ATable>();
	//Set the actor(this) as a parent
	table->AttachToActor(actorParent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//Change name according with the position it will occupy
	table->SetActorLocationAndRotation(FVector(0,0,10), FRotator());
#if WITH_EDITOR
	table->SetActorLabel("Table");
#endif

	CreateSetofChairs(world, actorParent);

	//Formula: (2*n+1)*chairWidth
	//For each chair there are one space on the left and one on the right, so that a chair takes as much space as 3 time its width
	nextUpSpawn = 5 * chairs[0]->GetSurfaceWidth();
	nextDownSpawn = 3 * chairs[0]->GetSurfaceWidth();
}

void ARoom::SetChairsPosition(int32 pivot, float value)
{
	if (chairs.Num() > 3)
	{
		//One chair needs enough space on the right and on the left
		int32 offsetChair = chairs[0]->GetSurfaceWidth() / 3;

		//Move all the chairs according with the direction of up/down scaling
		//In case there is enough space for new chairs,
		//these will be positionated on the near of the last spawned chair

		// c stadans for chair, the '>' dictate the direction of spawning
		//		c->
		//		----
		//	   ^|  |^
		//	   c|  |c
		//		----
		//		c->

		switch (pivot)
		{
		case 0:
			//Set chairs location. Not all chairs need to be updates, it depends on the pivot moved
			for (int32 i = 0; i < chairs.Num() / 4; i++)
			{
				chairs[i * 4]->SetActorLocation(chairs[i * 4]->GetActorLocation() + FVector(-value,-value,  0));
				chairs[i * 4 + 1]->SetActorLocation(chairs[i * 4 + 1]->GetActorLocation() + FVector(-value, 0, 0));
				chairs[i * 4 + 2]->SetActorLocation(chairs[i * 4 + 2]->GetActorLocation() + FVector(0, -value, 0));
				chairs[i * 4 + 3]->SetActorLocation(chairs[i * 4 + 3]->GetActorLocation() + FVector(-value,-value,  0));
			}
			break;
		case 1:
			for (int32 i = 0; i < chairs.Num() / 4; i++)
			{
				chairs[i * 4]->SetActorLocation(chairs[i * 4]->GetActorLocation() + FVector(-value, 0, 0));
				chairs[i * 4 + 1]->SetActorLocation(chairs[i * 4 + 1]->GetActorLocation() + FVector(-value, value, 0));
				chairs[i * 4 + 3]->SetActorLocation(chairs[i * 4 + 3]->GetActorLocation() + FVector(-value, 0, 0));
			}
			break;
		case 2:
			for (int32 i = 0; i < chairs.Num() / 4; i++)
			{
				chairs[i * 4 + 1]->SetActorLocation(chairs[i * 4 + 1]->GetActorLocation() + FVector(0, value, 0));
				chairs[i * 4 + 2]->SetActorLocation(chairs[i * 4 + 2]->GetActorLocation() + FVector(value, 0, 0));
			}
			break;
		case 3:
			for (int32 i = 0; i < chairs.Num() / 4; i++)
			{

				chairs[i * 4]->SetActorLocation(chairs[i * 4]->GetActorLocation() + FVector(0, -value, 0));
				chairs[i * 4 + 2]->SetActorLocation(chairs[i * 4 + 2]->GetActorLocation() + FVector(value, -value, 0));
				chairs[i * 4 + 3]->SetActorLocation(chairs[i * 4 + 3]->GetActorLocation() + FVector(0, -value, 0));
			}
			break;
		default:
			break;
		}
	}
}

void ARoom::AddChairs(int32 pivot)
{
	if (table->GetSurfaceWidth() >= nextUpSpawn)
	{
		//Create a set of chairs
		CreateSetofChairs(GetWorld(), this);

		int32 surface = chairs[0]->GetSurfaceWidth();

		if (chairs.Num() > 7)
		{
			//Set the location of the new chairs
			chairs[chairs.Num() - 4]->SetActorLocation(chairs[chairs.Num() - 8]->GetActorLocation() + FVector(0, surface * 2, 0));
			chairs[chairs.Num() - 3]->SetActorLocation(chairs[chairs.Num() - 7]->GetActorLocation() + FVector(surface * 2, 0, 0));
			chairs[chairs.Num() - 2]->SetActorLocation(chairs[chairs.Num() - 6]->GetActorLocation() + FVector(0, surface * 2, 0));
			chairs[chairs.Num() - 1]->SetActorLocation(chairs[chairs.Num() - 5]->GetActorLocation() + FVector(surface * 2, 0, 0));
		}

		nextUpSpawn += 2 * surface;
		nextDownSpawn += 2 * surface;
	}
}
void ARoom::RemoveChairs()
{
	if (table->GetSurfaceWidth() <= nextDownSpawn && chairs.Num()>3 && table->GetSurfaceWidth() > 3.1f * chairs[0]->GetSurfaceWidth())
	{
		int32 surface = chairs[0]->GetSurfaceWidth();
		int32 n = chairs.Num();
		for (int32 i = n - 4; i < n; i++)
		{
			AChair* tmp = chairs.Pop();
			tmp->Delete();
		}
		nextDownSpawn -= 2 * surface;
		nextDownSpawn = nextDownSpawn < 3 * surface ? 3 * surface : nextDownSpawn;
		nextUpSpawn -= 2 * surface;
	}
}

//Create a set of 4 chairs and set the location
//For the first set this is done with relative positions respect the table
//From the second above their positions are set outside from this method (the position will be relative to the existing chairs)
void ARoom::CreateSetofChairs(UWorld* world, AActor* actorParent) {
	AChair* tmp;
	TArray<FVector> positions;
	//All 4 different rotations
	TArray<FRotator> rotations = {FRotator(0,0,0),FRotator(0,-90,0) ,FRotator(0,-180,0) ,FRotator(0,-270,0) };
	for (int32 i = 0; i < 4; i++)
	{
		tmp = world->SpawnActor<AChair>();
		tmp->AttachToActor(actorParent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		//Offset to make the chiars not collide with the table
		int32 offsetChair = tmp->GetSurfaceWidth() / 3;
		//Set initial locations
		positions = i == 0 ? TArray<FVector>{FVector(-offsetChair, tmp->GetSurfaceWidth(), 0),
			FVector(tmp->GetSurfaceWidth(), table->GetSurfaceWidth() + offsetChair, 0),
			FVector(table->GetSurfaceWidth() + offsetChair, tmp->GetSurfaceWidth() + tmp->GetSurfaceWidth(), 0),
			FVector(tmp->GetSurfaceWidth() + tmp->GetSurfaceWidth(), -offsetChair, 0)
		} : positions;
		tmp->SetActorLocationAndRotation(positions[i], rotations[i]);
		//SetActorLabel works only in the editor. It lauches an exception when you try to create .exe
#if WITH_EDITOR
		FString name = "Chair: " + FString::FromInt(chairs.Num() + 1);
		tmp->SetActorLabel(*name);
#endif
		chairs.Add(tmp);
	}
}

//Destoy Actor and children
void ARoom::Delete(bool value) {
	for (int32 i = 0; i < chairs.Num(); i++)
	{
		chairs[i]->Delete();
	}
	table->Delete();
	Destroy();
}

ATable * ARoom::GetTable()
{
	return table;
}

int32 ARoom::GetChairWidth()
{
	return chairs.Num() > 0 ? chairs[0]->GetSurfaceWidth() : -1;
}

// Called every frame
void ARoom::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);
}

