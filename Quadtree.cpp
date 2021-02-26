// Fill out your copyright notice in the Description page of Project Settings.


#include "Quadtree.h"
#include "RuntimeMeshComponent.h"

Quadtree::Quadtree(){}

Quadtree::Quadtree(AMyActor* in, FVector localUp, int Radius, int LodLevel) {

	this->LodLevel = LodLevel;
	this->StaticProvider = NewObject<URuntimeMeshProviderStatic>(in);
	Direction = localUp;
	this->QT_Actor = in;

	this->AxisA = FVector(localUp.Y, localUp.Z, localUp.X);
	this->AxisB = FVector::CrossProduct(localUp, AxisA);
	this->Radius = Radius;
	this->RootNode = QuadtreeNode(nullptr, localUp*Radius, Radius, LodLevel, Direction, AxisA, AxisB);
	this->RootNode.GenerateNodeMesh(QT_Actor, StaticProvider, Direction, LodLevel);

	UE_LOG(LogTemp, Warning, TEXT("LocalUp is %s"), *Direction.ToString());
}

Quadtree::~Quadtree(){}





int Quadtree::GetDistance(FVector A, FVector B) {
	return FGenericPlatformMath::Sqrt(FGenericPlatformMath::Pow(A.X - B.X, 2.0f) + FGenericPlatformMath::Pow(A.Y - B.Y, 2.0f) + FGenericPlatformMath::Pow(A.Z - B.Z, 2));
}


void Quadtree::GenerateMesh(QuadtreeNode CurrentNode) {
	int Threshold = 10000;
	
	FVector BoxCenter,origin;
	QT_Actor->GetActorBounds(true, origin, BoxCenter);

	FVector CameraPos = QT_Actor->GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("Resolution is %s"), *CameraPos.ToString());

	int distance = GetDistance(CameraPos, origin);
	UE_LOG(LogTemp, Warning, TEXT("Distance from planet is %d"), distance);
	UE_LOG(LogTemp, Warning, TEXT("Threshold is %d"), Threshold);

	UE_LOG(LogTemp, Warning, TEXT("Lod is %d"), LodLevel);
	if (CurrentNode.GetLodLevel() < 3 && CurrentNode.GetLodLevel() >= 0) {
		if (distance <= Threshold) {
			InitialiseNodes(CurrentNode.parentNode);
		}
	}
	else{
		CurrentNode.GenerateNodeMesh(QT_Actor, StaticProvider, Direction, LodLevel);
	}
}

//Splits Node into four quadrants
void Quadtree::InitialiseNodes(QuadtreeNode* parentNode){
	int halfRadius = Radius;
	// Bottom right node
	if (!parentNode) {
		halfRadius = parentNode->Radius / 2;
	}


	FVector brPos = Direction + (AxisA * halfRadius + AxisB * halfRadius);
	UE_LOG(LogTemp, Warning, TEXT("inQTN direction is %s"), *brPos.ToString());
	
	QuadtreeNode br = QuadtreeNode(parentNode, brPos, halfRadius, this->LodLevel + 1, this->Direction, AxisA,AxisB);
	GenerateMesh(br);
	// Bottom left node
	FVector blPos = Direction + (AxisA * halfRadius - AxisB * halfRadius);
	QuadtreeNode bl = QuadtreeNode(parentNode, blPos, halfRadius, this->LodLevel + 1, this->Direction, AxisA, AxisB);
	GenerateMesh(bl);

	// Top right node
	FVector trPos = Direction - (AxisA * halfRadius + AxisB * halfRadius);

	QuadtreeNode tr = QuadtreeNode(parentNode, trPos, halfRadius, this->LodLevel + 1, this->Direction, AxisA, AxisB);
	GenerateMesh(tr);
	// Top left node
	FVector tlPos = Direction - (AxisA * halfRadius - AxisB * halfRadius);

	QuadtreeNode tl = QuadtreeNode(parentNode, tlPos, halfRadius, this->LodLevel + 1, this->Direction, AxisA, AxisB);
	GenerateMesh(tl);
}





