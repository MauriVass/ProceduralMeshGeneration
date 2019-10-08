//Chair model created in a procedural way

#include "Chair.h"

// Sets default values
AChair::AChair()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	chair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chair"));
	RootComponent = chair;
}

int32 AChair::GetSurfaceWidth() {
	return AChair::surfaceWidth;
}
int32 AChair::GetSpacing() {
	return AChair::spacing;
}

// Called when the game starts or when spawned
void AChair::BeginPlay()
{
	Super::BeginPlay();

	int32 legWidth = 6;
	int32 legHeight = 70;

	int32 barWidth =  4;
	int32 barHeight = 80;

	surfaceWidth = 80;
	int32 surfaceHeight = 8;

	int32 backRestHeight = 6;
	//Make the legs be near the center of the surface (DON'T SET IT TOO SMALL!)
	float offset = 0.95f;

	//The space needed between leg table and chair and between each chair
	//In this case is equal to the surfaceWidth but can be changed
	spacing = surfaceWidth;

	/***LEGS***/
	ABoxModel::CreatePiece(GetWorld(), FVector(surfaceWidth * (1 - offset), surfaceWidth * (1 - offset), 0), FRotator(0,0,0), legWidth, legHeight, true, "FrontLeftLeg", this);
	ABoxModel::CreatePiece(GetWorld(), FVector(surfaceWidth * (1 - offset), surfaceWidth * offset - legWidth, 0), FRotator(0,0,0), legWidth, legHeight, true, "FrontRightLeg", this);
	ABoxModel::CreatePiece(GetWorld(), FVector(surfaceWidth * offset - legWidth, surfaceWidth * offset - legWidth, 0), FRotator(0,0,0), legWidth, legHeight, true, "RearRightLeg", this);
	ABoxModel::CreatePiece(GetWorld(), FVector(surfaceWidth * offset - legWidth, surfaceWidth * (1 - offset), 0), FRotator(0,0,0), legWidth, legHeight, true, "RearLeftLeg", this);

	/***SURFACE FOR SEAT***/
	//There is an hard coded float (0.95f) so the upper faces of the legs are inside the surface box 
	ABoxModel::CreatePiece(GetWorld(), FVector(0, 0, legHeight * 0.95f), FRotator(0,0,0), surfaceWidth, surfaceHeight, false, "Surface", this);

	/***UPPER BARS***/
	ABoxModel::CreatePiece(GetWorld(), FVector(surfaceWidth * (1 - offset), surfaceWidth * (1 - offset), (legHeight + surfaceHeight) * 0.95f), FRotator(0,0,0), barWidth, legHeight, true, "UpperLeftBar", this);
	ABoxModel::CreatePiece(GetWorld(), FVector(surfaceWidth * (1 - offset), surfaceWidth * offset - legWidth, (legHeight + surfaceHeight) * 0.95f), FRotator(0,0,0), barWidth, legHeight, true, "UpperRightBar", this);

	/***BACKREST***/
	ABoxModel::CreatePiece(GetWorld(), FVector(1.5f * barWidth + surfaceHeight,0,legHeight+barHeight/4), FRotator(90, 0, 0), surfaceWidth,backRestHeight,false,"Backrest",this);

	//Seem to doesn't work
	//this->SetPivotOffset(FVector(surfaceWidth/2,surfaceWidth/2,0));
}

// Called every frame
void AChair::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

}

