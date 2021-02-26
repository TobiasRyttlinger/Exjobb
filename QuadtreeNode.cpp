#include "QuadtreeNode.h"
#include "DrawDebugHelpers.h"


QuadtreeNode::QuadtreeNode()
{
}

/** Shallow copies a QuadtreeNode. */
QuadtreeNode::QuadtreeNode(const QuadtreeNode& copy)
{
	SetParentNode(copy.parentNode);
	childNodes = copy.childNodes;
}

QuadtreeNode::~QuadtreeNode()
{
}


QuadtreeNode::QuadtreeNode(QuadtreeNode *Parent,FVector Position, float Radius, int LodLevel, FVector localUp, FVector axisA, FVector axisB){
	this->parentNode = Parent;
	this->Radius = Radius;
	this->LodLevel = LodLevel;
	this->localUp = localUp;
	this->AxisA = axisA;
	this->AxisB = axisB;
	this->Position = Position;
	UE_LOG(LogTemp, Warning, TEXT("LocalUp in Node is %s"), *Position.ToString());
}


void QuadtreeNode::SetParentNode(QuadtreeNode* weakParentNode)
{
	this->parentNode = weakParentNode;
}

QuadtreeNode* QuadtreeNode::GetParentNode()
{
	return parentNode;
}

void QuadtreeNode::AddChildNode(QuadtreeNode node)
{
	childNodes.Add(node);
}

TArray<QuadtreeNode> QuadtreeNode::GetChildNodes()
{
	return childNodes;
}

void QuadtreeNode::setMeshPosition(FVector newPos) {
	this->MeshPosition = newPos;
}


bool QuadtreeNode::HasChildNodes() const
{
	return childNodes.Num() > 0;
}


int QuadtreeNode::GetLodLevel() {
	return LodLevel;
}


void QuadtreeNode::GenerateNodeMesh(AMyActor* in, URuntimeMeshProviderStatic* StaticProvider, FVector LocalUp, int Lod ) {
	
	if (this->HasChildNodes()) {
		//childNodes.Empty();
		//RMC->ClearSection(0, this->LodLevel - 1);

	}
	

	RMC = NewObject<URuntimeMeshComponentStatic>(in);

	RMC->RegisterComponent();


	RMC->Initialize(StaticProvider);
	StaticProvider->SetupMaterialSlot(0, TEXT("Material"), nullptr);

	TArray<FVector> Vertices;
	TArray<int> Triangles;

	
	UE_LOG(LogTemp, Warning, TEXT("LocalUp in GenVec is %s"), *LocalUp.ToString());
	
	int NoiseSamplesPerLine = 64;

	//UE_LOG(LogTemp, Warning, TEXT("Text, %d %f %s"), GetLodLevel());
	float NoiseInputScale = 0.01; // Making this smaller will "stretch" the perlin noise terrain
	float NoiseOutputScale = 2000; // Making this bigger will scale the terrain's height

	// This creates 3 vertex colors
	TArray<FColor> Colors{ FColor::Blue, FColor::Red, FColor::Green, FColor::White };
	// This indexes our simple triangle
	Vertices = GenerateVertices(in,Vertices, NoiseSamplesPerLine, LocalUp);
	Triangles = GenerateTriangles(NoiseSamplesPerLine, Triangles);

	// Empty for now
	TArray<FVector> EmptyNormals;
	TArray<FVector2D> EmptyTexCoords;
	TArray<FRuntimeMeshTangent> EmptyTangents;
	StaticProvider->CreateSectionFromComponents(0, Lod, 0, Vertices, Triangles, EmptyNormals, EmptyTexCoords, Colors, EmptyTangents, ERuntimeMeshUpdateFrequency::Infrequent, true);

}

TArray<FVector> QuadtreeNode::GenerateVertices(AMyActor* in, TArray<FVector>Vertices, int Resolution,FVector LocalUp) {
	Vertices.Init(FVector(0,0,0), Resolution * Resolution); // 64x64

	FVector axisA = FVector(LocalUp.Y, LocalUp.Z, LocalUp.X);
	FVector axisB = FVector::CrossProduct(LocalUp, axisA);

	for (int y = 0; y < Resolution; y++) {
		for (int x = 0; x < Resolution; x++) {
			int index = GetIndexForGridCoordinates(x, y, Resolution);
	
			FVector2D Percentage = FVector2D(x, y) /( Resolution-1);	

			//	UE_LOG(LogTemp, Warning, TEXT("LocalUp is %d"), LodLevel);
			FVector PointOnCube = LocalUp + (Percentage.X - 0.5f)*2*axisA + (Percentage.Y - 0.5f)*2* axisB;
			PointOnCube.Normalize(1.0f);
			//UE_LOG(LogTemp, Warning, TEXT("LocalUp is %s"), *Position.ToString());
			FVector PointOnSphere = PointOnCube*in->PlanetSize;
			Vertices[index] = PointOnSphere;
		}
	}

	return Vertices;
}

/*// Returns the scaled noise value for grid coordinates [x,y]
float GetNoiseValueForGridCoordinates(int x, int y) {
	return Noise->GetValue(
		(x * NoiseInputScale) + 0.1,
		(y * NoiseInputScale) + 0.1
	) * NoiseOutputScale;
}
*/

int QuadtreeNode::GetIndexForGridCoordinates(int x, int y, int NoiseSamplesPerLine) {
	return x + y * NoiseSamplesPerLine;
}

FVector2D QuadtreeNode::GetPositionForGridCoordinates(int x, int y, int NoiseResolution) {
	return FVector2D(
		x * NoiseResolution,
		y * NoiseResolution
	);
}

TArray<int>  QuadtreeNode::GenerateTriangles(int NoiseSamplesPerLine, TArray<int>Triangles) {
	
	int NumberOfQuadsPerLine = NoiseSamplesPerLine - 1; // We have one less quad per line than the amount of vertices, since each vertex is the start of a quad except the last ones
	// In our triangles array, we need 6 values per quad
	int TrianglesArraySize = NumberOfQuadsPerLine * NumberOfQuadsPerLine * 6;
	Triangles.Init(0, TrianglesArraySize);
	int TriangleIndex = 0;

	for (int y = 0; y < NumberOfQuadsPerLine; y++) {
		for (int x = 0; x < NumberOfQuadsPerLine; x++) {

			// Getting the indexes of the four vertices making up this quad
			int bottomLeftIndex = GetIndexForGridCoordinates(x, y, NoiseSamplesPerLine);
			int topLeftIndex = GetIndexForGridCoordinates(x, y + 1, NoiseSamplesPerLine);
			int topRightIndex = GetIndexForGridCoordinates(x + 1, y + 1, NoiseSamplesPerLine);
			int bottomRightIndex = GetIndexForGridCoordinates(x + 1, y, NoiseSamplesPerLine);

			// Assigning the 6 triangle points to the corresponding vertex indexes, by going counter-clockwise.
			Triangles[TriangleIndex] = bottomLeftIndex;
			Triangles[TriangleIndex + 1] = topLeftIndex;
			Triangles[TriangleIndex + 2] = topRightIndex;
			Triangles[TriangleIndex + 3] = bottomLeftIndex;
			Triangles[TriangleIndex + 4] = topRightIndex;
			Triangles[TriangleIndex + 5] = bottomRightIndex;
			TriangleIndex += 6;
		}
	}
	return Triangles;
}



