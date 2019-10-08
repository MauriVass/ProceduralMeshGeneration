//Table model created in a procedural way

#include "Table.h"

// Sets default values
ATable::ATable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	table = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Table"));
	RootComponent = table;
}

	int32 ATable::GetSurfaceWidth() {
		return surfaceWidth;
	}

// Called when the game starts or when spawned
void ATable::BeginPlay()
{
	Super::BeginPlay();
	
	int32 legWidth = 10;
	int32 legHeight = 100;
	//Three times chair size
	surfaceWidth = 240;
	int32 surfaceHeight = 8;
	//Make the legs be near the center of the surface (DON'T SET IT TOO SMALL!)
	float offset = 0.95f;

	/***LEGS***/
	ABoxModel::CreatePiece(GetWorld(), FVector(surfaceWidth * (1 - offset), surfaceWidth * (1 - offset), 0),FRotator(0,0,0), legWidth, legHeight, true,"FrontLeftLeg",this);
	ABoxModel::CreatePiece(GetWorld(), FVector(surfaceWidth * (1 - offset), surfaceWidth * offset - legWidth, 0),FRotator(0,0,0), legWidth, legHeight, true, "FrontRightLeg", this);
	ABoxModel::CreatePiece(GetWorld(), FVector(surfaceWidth * offset - legWidth, surfaceWidth * offset - legWidth, 0),FRotator(0,0,0), legWidth, legHeight, true, "RearRightLeg", this);
	ABoxModel::CreatePiece(GetWorld(), FVector(surfaceWidth * offset - legWidth, surfaceWidth * (1 - offset), 0),FRotator(0,0,0), legWidth, legHeight, true, "RearLeftLeg", this);

	/***SURFACE***/
	//There is an hard coded float (0.95f) so the upper faces of the legs are inside the surface box 
	ABoxModel::CreatePiece(GetWorld(), FVector(0, 0, legHeight * 0.95f),FRotator(0,0,0), surfaceWidth,surfaceHeight,false,"Surface",this);
}

// Called every frame
void ATable::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);
}

