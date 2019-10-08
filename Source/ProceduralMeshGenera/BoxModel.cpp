//Create a simple box in a procedural way
//A box can be a parallelepided(2 equal sides and one different) or a cube(all equal sides)

#include "BoxModel.h"

// Sets default values
ABoxModel::ABoxModel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	box = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = box;
}

// Called when the game starts or when spawned
void ABoxModel::BeginPlay()
{
	Super::BeginPlay();

	//Set material so that a texture can be applied
	//Can change texture in the editor
	box->SetMaterial(0, material);
}



void ABoxModel::AddVertex(FVector position) {
	vertices.Add(position);
}

void ABoxModel::AddTriangle(int32 v1, int32 v2, int32 v3) {
	triangles.Add(v1);
	triangles.Add(v2);
	triangles.Add(v3);
}

void ABoxModel::GenerateBox(int32 width, int32 height, bool leg) {
	int32 w, h;
	w = width;
	h = height;

	//Add all the verteces of the procedural mesh
	//Each vertex has a FVector position -> coordinate in 3D space
	AddVertex(FVector(0, 0, 0));//down left front corner 0
	AddVertex(FVector(0, w, 0));//down right front corner 1
	AddVertex(FVector(w, w, 0));//down right rear corner 2
	AddVertex(FVector(w, 0, 0));//down left rear corner 3

	AddVertex(FVector(0, 0, h));//up left front corner 4
	AddVertex(FVector(0, w, h));//up right front corner 5
	AddVertex(FVector(w, w, h));//up right rear corner 6
	AddVertex(FVector(w, 0, h));//up left rear corner 7

	//Create a triangle passing the position (index of vertices array. ORDER OF INSERTING IS IMPORTANT!) of 3 vertices
	//Pass vetices in clock-wise or counter clock-wise order to decided the direction of the normal at the triangle
	//Front Face - Counter clock-wise
	AddTriangle(0, 1, 5);
	AddTriangle(0, 5, 4);

	//Right Face - Counter clock-wise
	AddTriangle(1, 2, 6);
	AddTriangle(6, 5, 1);

	//Rear Face - Clock-wise
	AddTriangle(2, 3, 7);
	AddTriangle(7, 6, 2);

	//Left Face - Clock-wise
	AddTriangle(0, 4, 7);
	AddTriangle(7, 3, 0);

	//Down Face - Clock-wise
	AddTriangle(0, 3, 2);
	AddTriangle(2, 1, 0);

	//If the shape is a leg it doesn't need the upper face since it is not visible from the 'player'
	if (!leg)
	{
		//Up Face - Counter Clock-wise
		AddTriangle(4, 5, 6);
		AddTriangle(6, 7, 4);
	}

	//Create mesh passing: an index, vertices, trinagles and other optional parameters
	box->CreateMeshSection_LinearColor(0, vertices, triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), false);
}

void ABoxModel::CreatePiece(UWorld* world, FVector position, FRotator rotation, int32 width, int32 height, bool isLeg, FString name, AActor* actorParent) {
	//Store memory for one table shape
	ABoxModel* tmp = world->SpawnActor<ABoxModel>();
	//Set the actor(this) as a parent
	tmp->AttachToActor(actorParent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//Change its position and rotation after it is attached to the parent
	tmp->SetActorLocationAndRotation(position, rotation);
	//Generate mesh with width and height specified and if it a leg or not
	tmp->GenerateBox(width, height, isLeg);
	//Change name according with the position it will occupy
	tmp->SetActorLabel(name);
}


// Called every frame
void ABoxModel::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);
}

