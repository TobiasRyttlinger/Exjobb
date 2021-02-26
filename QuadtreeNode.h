#pragma once

// Custom includes
#include "Templates/SharedPointer.h"
#include "Components/RuntimeMeshComponentStatic.h"
#include "Providers/RuntimeMeshProviderStatic.h"
#include "Providers/RuntimeMeshProviderSphere.h"
#include "Containers/Array.h"
#include "MyActor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Math/Vector.h"
// Core includes
#include "CoreMinimal.h"


class EXJOBB2_API QuadtreeNode
{
public:
	QuadtreeNode(QuadtreeNode* Parent,FVector Position, float Radius, int LodLevel,FVector localUp, FVector axisA, FVector axisB);
	QuadtreeNode();
	QuadtreeNode(const QuadtreeNode& copy);
	~QuadtreeNode();


	/** Set the parent node for this node. */
	void SetParentNode(QuadtreeNode* parentNode);

	/** Get the parent node to this node. */
	QuadtreeNode* GetParentNode();

	/* Add a child node to this node. */
	void AddChildNode(QuadtreeNode node);

	/** Get child nodes belonging to this node. */
	TArray<QuadtreeNode> GetChildNodes();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		URuntimeMeshComponentStatic* RMC;

	//Mesh generating function and helper functions

	void GenerateNodeMesh(AMyActor* in, URuntimeMeshProviderStatic* StaticProvider, FVector LocalUp, int LodLevel);

	TArray<FVector> GenerateVertices(AMyActor* in, TArray<FVector>Vertices, int Resolution, FVector LocalUp);

	int GetIndexForGridCoordinates(int x, int y, int NoiseSamplesPerLine);

	FVector2D GetPositionForGridCoordinates(int x, int y, int NoiseResolution);

	TArray<int> GenerateTriangles(int NoiseSamplesPerLine, TArray<int>Triangles);


	/** Get the distance from the root node of the tree. */


	int GetLodLevel();

	float Radius;

	/** Check if this node has any children nodes. */
	bool HasChildNodes() const;

	/** Check if this node has any children nodes. */

	void setMeshPosition(FVector newPos);

	QuadtreeNode* parentNode;
	FVector localUp;
private:
	/** Get the root node of this quadtree. */
	QuadtreeNode GetRootNode();



private:
	
	FVector2D MeshCenter;



	/** The parent node to this node */


	FVector Position;
	
	int LodLevel;
	
	FVector AxisA; 
	FVector AxisB;

	/** Child nodes held by this node. */
	TArray<QuadtreeNode> childNodes;



	FVector MeshPosition;

};