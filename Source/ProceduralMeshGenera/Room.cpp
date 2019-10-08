//Collection of table and chairs

#include "Room.h"

// Sets default values
ARoom::ARoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	room = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Room"));
	RootComponent = room;

	chairs = TArray<AChair*>();
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
	table->SetActorLocationAndRotation(FVector(), FRotator());
	table->SetActorLabel("Table");

	CreateSetofChairs(world, actorParent);
}

void ARoom::CreateSetofChairs(UWorld* world, AActor* actorParent) {
	AChair* tmp;
	TArray<FVector> positions;
	TArray<FRotator> rotations = {FRotator(0,0,0),FRotator(0,-90,0) ,FRotator(0,-180,0) ,FRotator(0,-270,0) };
	int32 n = chairs.Num() % 4;
	for (int32 i = 0; i < 4; i++)
	{
		tmp = world->SpawnActor<AChair>();
		tmp->AttachToActor(actorParent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		//Offset to make the chiars not collide with the table
		int32 offsetChair = tmp->GetSurfaceWidth() / 3;
		positions = i == 0 ? TArray<FVector>{FVector(-offsetChair, (2 * n + 1)*tmp->GetSurfaceWidth(), 0),
			FVector((2 * n + 1)*tmp->GetSurfaceWidth(), table->GetSurfaceWidth() + offsetChair, 0),
			FVector(table->GetSurfaceWidth() + offsetChair, (2 * n + 1)*tmp->GetSurfaceWidth() + tmp->GetSurfaceWidth(), 0),
			FVector((2 * n + 1)*tmp->GetSurfaceWidth() + tmp->GetSurfaceWidth(), -offsetChair, 0)
		} : positions;
		tmp->SetActorLocationAndRotation(positions[i], rotations[i]);
		FString name = "Chair: " + FString::FromInt(chairs.Num() + 1);
		tmp->SetActorLabel(*name);
		chairs.Add(tmp);
	}
}

// Called every frame
void ARoom::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

}

