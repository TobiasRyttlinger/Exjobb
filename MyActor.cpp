// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Quadtree.h"



Quadtree QT_1;
Quadtree QT_2;
Quadtree QT_3;
Quadtree QT_4;
Quadtree QT_5;
Quadtree QT_6;

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
    Super::BeginPlay();

    FVector up = this->GetActorUpVector();
    FVector right = this->GetActorRightVector();
    FVector forward = this->GetActorForwardVector();

    FVector down = -up;
    FVector left = -right;
    FVector back = -forward;



    QT_1 = Quadtree(this, up, this->PlanetSize, 0);
    //QT_2 = Quadtree(this, down, this->PlanetSize, 0);
    //QT_3 = Quadtree(this, right, this->PlanetSize, 0);
    //QT_4 = Quadtree(this, forward, this->PlanetSize, 0);
    //QT_5 = Quadtree(this, left, this->PlanetSize, 0);
    //QT_6 = Quadtree(this, back, this->PlanetSize, 0);
    
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
    QT_1.GenerateMesh(QT_1.RootNode);
    //QT_2.GenerateMesh(QT_2.RootNode);
    //QT_3.GenerateMesh(QT_3.RootNode);
    //QT_4.GenerateMesh(QT_4.RootNode);
    //QT_5.GenerateMesh(QT_5.RootNode);
    //QT_6.GenerateMesh(QT_6.RootNode);


	Super::Tick(DeltaTime);
}

