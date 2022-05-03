// Fill out your copyright notice in the Description page of Project Settings.

#include "PlantBunchManager.h"

// Sets default values
APlantBunchManager::APlantBunchManager()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlantBunchManager::BeginPlay()
{
  Super::BeginPlay();
}

// Called every frame
void APlantBunchManager::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

float GetRandomReal(float low, float up)
{
   float fResult;
  if (low > up)
  {
    float temp = low;
    low = up;
    up = temp;
  }
  fResult = low + (up - low) * rand() /(double) (RAND_MAX );
    UE_LOG(LogClass, Log, TEXT("GetRandomReal:%f"),fResult);
  return fResult;
}

void APlantBunchManager::Generate()
{
  FTransform spawnLocation;
  GetWorld()->SpawnActor<AActor>(plantBunch->mainTree->GeneratedClass, spawnLocation);
  // generate first subtree
    UE_LOG(LogClass, Log, TEXT("radius:%f"),plantBunch->radius);
  for (int i = 0; i < plantBunch->number0; i++)
  {
    FVector pos = spawnLocation.GetLocation();
    
    float deltaX = GetRandomReal(-plantBunch->radius,plantBunch->radius);
    float deltaY = GetRandomReal(-plantBunch->radius,plantBunch->radius);

    UE_LOG(LogClass, Log, TEXT("generate sub0 deltaX:%f, deltaY:%f"),deltaX,deltaY);
    FVector newPos(pos.X +deltaX, pos.Y +  deltaY, pos.Z);
    FTransform spawnLocation0(newPos);
    GetWorld()->SpawnActor<AActor>(plantBunch->subTree0->GeneratedClass, spawnLocation0);
  }

  for (int i = 0; i < plantBunch->number1; i++)
  {
    FVector pos = spawnLocation.GetLocation();
    float deltaX = GetRandomReal(-plantBunch->radius,plantBunch->radius);
    float deltaY = GetRandomReal(-plantBunch->radius,plantBunch->radius);

      UE_LOG(LogClass, Log, TEXT("generate sub1 deltaX:%f, deltaY:%f"),deltaX,deltaY);
    FVector newPos(pos.X +deltaX, pos.Y +  deltaY, pos.Z);
    FTransform spawnLocation0(newPos);
    GetWorld()->SpawnActor<AActor>(plantBunch->subTree1->GeneratedClass, spawnLocation0);
  }

  for (int i = 0; i < plantBunch->number2; i++)
  {
    FVector pos = spawnLocation.GetLocation();
    float deltaX = GetRandomReal(-plantBunch->radius,plantBunch->radius);
    float deltaY = GetRandomReal(-plantBunch->radius,plantBunch->radius);

      UE_LOG(LogClass, Log, TEXT("generate sub2 deltaX:%f, deltaY:%f"),deltaX,deltaY);
    FVector newPos(pos.X +deltaX, pos.Y +  deltaY, pos.Z);
    FTransform spawnLocation0(newPos);
    
    GetWorld()->SpawnActor<AActor>(plantBunch->subTree2->GeneratedClass, spawnLocation0);
  }

  UE_LOG(LogClass, Log, TEXT("Generate.%s"));
}
