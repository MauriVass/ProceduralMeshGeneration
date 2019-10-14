//Table model created in a procedural way

#include "Table.h"

// Sets default values
ATable::ATable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	table = CreateDefaultSubobject<USceneComponent>(TEXT("Table"));
	RootComponent = table;

	legWidth = 10;
	legHeight = 100;
	//Three times chair size
	surfaceWidth = 240;
	surfaceHeight = 8;

	pivotWidth = legWidth * 1.5f;
	//Make the legs be near the center of the surface (DON'T SET IT TOO SMALL!)
	offset = 0.95f;
}

ABoxModel* ATable::GetSurface()
{
	return surface;
}
int32 ATable::GetSurfaceWidth() {
	return surfaceWidth;
}


ABoxModel* ATable::GetPivot(int index)
{
	return pivots[index];
}
ABoxModel* ATable::GetCentralPivot()
{
	return centralPivot;
}

// Called when the game starts or when spawned
void ATable::BeginPlay()
{
	Super::BeginPlay();

	/***LEGS***/
	legs.Add(ABoxModel::CreatePiece(GetWorld(), FVector(surfaceWidth * (1 - offset), surfaceWidth * (1 - offset), 0), FRotator(0, 0, 0), legWidth, legHeight, true, "FrontLeftLeg", this));
	legs.Add(ABoxModel::CreatePiece(GetWorld(), FVector(surfaceWidth * (1 - offset), surfaceWidth * offset - legWidth, 0), FRotator(0, 0, 0), legWidth, legHeight, true, "FrontRightLeg", this));
	legs.Add(ABoxModel::CreatePiece(GetWorld(), FVector(surfaceWidth * offset - legWidth, surfaceWidth * offset - legWidth, 0), FRotator(0, 0, 0), legWidth, legHeight, true, "RearRightLeg", this));
	legs.Add(ABoxModel::CreatePiece(GetWorld(), FVector(surfaceWidth * offset - legWidth, surfaceWidth * (1 - offset), 0), FRotator(0, 0, 0), legWidth, legHeight, true, "RearLeftLeg", this));
	
	/***SURFACE***/
	//There is an hard coded float value(0.95f) so the upper faces of the legs are inside the surface box 
	surface = ABoxModel::CreatePiece(GetWorld(), FVector(0, 0, legHeight * 0.95f),FRotator(0,0,0), surfaceWidth,surfaceHeight,false,"Surface",this);

	/***PIVOTS***/
	pivots.Add(ABoxModel::CreatePiece(GetWorld(), FVector(0, 0, legHeight + surfaceHeight), FRotator(0, 0, 0), pivotWidth, pivotWidth, false, "Pivot0", this));
	pivots.Add(ABoxModel::CreatePiece(GetWorld(), FVector(0, surfaceWidth, legHeight + surfaceHeight), FRotator(0, 0, 0), pivotWidth, pivotWidth, false, "Pivot1", this));
	pivots.Add(ABoxModel::CreatePiece(GetWorld(), FVector(surfaceWidth, surfaceWidth, legHeight + surfaceHeight), FRotator(0, 0, 0), pivotWidth, pivotWidth, false, "Pivot2", this));
	pivots.Add(ABoxModel::CreatePiece(GetWorld(), FVector(surfaceWidth, 0, legHeight + surfaceHeight), FRotator(0, 0, 0), pivotWidth, pivotWidth, false, "Pivot3", this));
	for (int32 i = 0; i < legs.Num(); i++)
	{
		//Set the pivot as animated so that they can change location over time
		pivots[i]->SetShouldAnimate(true);
	}

	/***CENTRAL PIVOT***/
	centralPivot = ABoxModel::CreatePiece(GetWorld(), FVector(surfaceWidth / 2 - pivotWidth / 2, surfaceWidth / 2 - pivotWidth / 2, legHeight * 2 + surfaceHeight), FRotator(0, 0, 0), pivotWidth, pivotWidth, false, "CentralPivot", this);
	centralPivot->SetShouldAnimate(true);
}

void ATable::SetObjectsPosition(int32 pivot, float value) {
	//Increase or decrease surface length
	surfaceWidth += value;

	//Change position of the existing chairs according with which table corner is moving
	switch (pivot)
	{
	case 0:
		//Left Front Corner
		legs[3]->SetActorLocation(legs[3]->GetActorLocation() + FVector(0, -value, 0));
		legs[0]->SetActorLocation(legs[0]->GetActorLocation() + FVector(-value, -value, 0));
		legs[1]->SetActorLocation(legs[1]->GetActorLocation() + FVector(-value, 0, 0));
		centralPivot->SetActorLocation(centralPivot->GetActorLocation() + FVector(-value, -value, 0));
		pivots[3]->TranslateInitialPosition(FVector(0, -value, 0));
		pivots[0]->TranslateInitialPosition(FVector(-value, -value, 0));
		pivots[1]->TranslateInitialPosition(FVector(-value, 0, 0));
		break;
	case 1:
		//Right Front Corner
		legs[0]->SetActorLocation(legs[0]->GetActorLocation() + FVector(-value, 0, 0));
		legs[1]->SetActorLocation(legs[1]->GetActorLocation() + FVector(-value, value, 0));
		legs[2]->SetActorLocation(legs[2]->GetActorLocation() + FVector(0, value, 0));
		centralPivot->SetActorLocation(centralPivot->GetActorLocation() + FVector(-value, value, 0));
		pivots[0]->TranslateInitialPosition(FVector(-value, 0, 0));
		pivots[1]->TranslateInitialPosition(FVector(-value, value, 0));
		pivots[2]->TranslateInitialPosition(FVector(0, value, 0));
		break;
	case 2:
		//Right Back Corner
		legs[1]->SetActorLocation(legs[1]->GetActorLocation() + FVector(0, value, 0));
		legs[2]->SetActorLocation(legs[2]->GetActorLocation() + FVector(value, value, 0));
		legs[3]->SetActorLocation(legs[3]->GetActorLocation() + FVector(value, 0, 0));
		centralPivot->SetActorLocation(centralPivot->GetActorLocation() + FVector(value, value, 0));
		pivots[1]->TranslateInitialPosition(FVector(0, value, 0));
		pivots[2]->TranslateInitialPosition(FVector(value, value, 0));
		pivots[3]->TranslateInitialPosition(FVector(value, 0, 0));
		break;
	case 3:
		//Left Back Corner
		legs[2]->SetActorLocation(legs[2]->GetActorLocation() + FVector(value,0,  0));
		legs[3]->SetActorLocation(legs[3]->GetActorLocation() + FVector(value, -value, 0));
		legs[0]->SetActorLocation(legs[0]->GetActorLocation() + FVector(0,-value,  0));
		centralPivot->SetActorLocation(centralPivot->GetActorLocation() + FVector(value, -value, 0));
		pivots[2]->TranslateInitialPosition(FVector(value,0,  0));
		pivots[3]->TranslateInitialPosition(FVector(value, -value, 0));
		pivots[0]->TranslateInitialPosition(FVector(0,-value,  0));
		break;
	default:
		break;
	}
}

//Destroy Actor and all attached children
void ATable::Delete()
{
	TArray<USceneComponent*> c;
	table->GetChildrenComponents(true, c);
	for (int32 i = 0; i < c.Num(); i++)
	{
		c[i]->DestroyComponent();
	}
	Destroy();
}

// Called every frame
void ATable::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);
}

