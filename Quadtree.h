#pragma once

// Custom includes
#include "QuadtreeNode.h"
#include "Templates/SharedPointer.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Components/RuntimeMeshComponentStatic.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "CoreMinimal.h"

class EXJOBB2_API Quadtree
{

public:
	Quadtree();
	Quadtree(AMyActor* in,FVector direction,int Radius,int LodLevel);
	~Quadtree();


	void GenerateMesh(QuadtreeNode CurrentNode);

	QuadtreeNode RootNode;
	int Radius;
private:

	int GetDistance(FVector A, FVector B);
	void InitialiseNodes(QuadtreeNode* parentNode);




private:
	// Root Node

	AMyActor* QT_Actor;
	URuntimeMeshProviderStatic* StaticProvider;
	FVector Direction;
	
	
	FVector AxisA;
	FVector AxisB;

	int LodLevel;
};