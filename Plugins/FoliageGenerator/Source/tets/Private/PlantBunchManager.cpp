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
    fResult = low + (up - low) * rand() / (double)(RAND_MAX);
    UE_LOG(LogClass, Log, TEXT("GetRandomReal:%f"), fResult);
    return fResult;
}

int getRandomInt(int low, int up){
    // low and up include
    if (low > up)
    {
        int temp = low;
        low = up;
        up = temp;
    }
    return rand()%(up-low+1) + low;
    
}

// convert the angle to radians
float toRad(float angle){
    return angle*3.14/180;
}

void APlantBunchManager::GenerateSingle(FVector centerPos,float radius, float angle, int type=0){
    UE_LOG(LogClass, Log, TEXT("cos:%f"),  FMath::Cos(angle));
    UE_LOG(LogClass, Log, TEXT("sin:%f"),  FMath::Sin(angle));
    float deltaX = radius * FMath::Cos(angle);
    float deltaY = radius * FMath::Sin(angle);
    UE_LOG(LogClass, Log, TEXT("deltaX:%f"), deltaX);
    FVector newPos(centerPos.X + deltaX, centerPos.Y + deltaY, centerPos.Z);
    FTransform spawnLocation0(newPos);
    switch (type) {
        case 0:{
            // sub tree
            int chosenTree = getRandomInt(0,plantBunch->subTrees.Num()-1);
            GetWorld()->SpawnActor<AActor>(plantBunch->subTrees[chosenTree]->GeneratedClass, spawnLocation0);
            break;
        }
        case 1:{
            int chosenBush = getRandomInt(0,plantBunch->bushes.Num()-1);
            GetWorld()->SpawnActor<AActor>(plantBunch->bushes[chosenBush]->GeneratedClass, spawnLocation0);
            break;
        }
        case 2:{
            int chosenGrass = getRandomInt(0,plantBunch->grassFlowers.Num()-1);
            GetWorld()->SpawnActor<AActor>(plantBunch->grassFlowers[chosenGrass]->GeneratedClass, spawnLocation0);
            break;
        }
        default:
            break;
    }
    
//    UE_LOG(LogClass, Log, TEXT("chosenTree:%d"), chosenTree);
    
    
}

void APlantBunchManager::Generate()
{
 

    FVector pos = mainTree->GetActorLocation();
    UE_LOG(LogClass, Log, TEXT("radius:%f"), plantBunch->radius);
    float radius = plantBunch->radius;
    // generate bush
    switch (plantBunch->subTreeNum)
    {
        case 0:
        {
            // do nothing
            break;
        }
        case 1:
        {
            // generate one sub tree
            float angle = GetRandomReal(0.0f, 6.28f);
            
            UE_LOG(LogClass, Log, TEXT("angle:%f"), angle);
            GenerateSingle(pos, radius,angle);
            break;
        }
        case 2:{
            float angle = GetRandomReal(0.0f, 6.28f);
            float angle1 = GetRandomReal(toRad(30.0f),toRad(60.0f))+angle;
            UE_LOG(LogClass, Log, TEXT("angle:%f"), angle);
            UE_LOG(LogClass, Log, TEXT("angle1:%f"), angle1);
            float radius1 = GetRandomReal(0.3,0.8)*radius;
            GenerateSingle(pos, radius,angle);
            GenerateSingle(pos, radius1,angle1);
            break;
        }
            
        case 3:{
            float angle0 = GetRandomReal(0.0f, 6.28f);
            float angle1 = GetRandomReal(toRad(30.0f),toRad(60.0f))+angle0;
            float angle2 = GetRandomReal(toRad(30.0f),toRad(60.0f))+angle1;
            float radius0 = GetRandomReal(0.3,0.8)*radius;
            float radius1 = radius;
            float radius2 = GetRandomReal(0.3,0.8)*radius;
            GenerateSingle(pos, radius0,angle0);
            GenerateSingle(pos, radius1,angle1);
            GenerateSingle(pos, radius2,angle2);
            break;
        }
        default:
        {
            break;
        }
    }
    // generate bushes
    for (int i=0; i<plantBunch->bushNum; i++) {
        float radius0 = GetRandomReal(0.0,1.0)*radius;
        float angle0 = GetRandomReal(0.0f, 6.28f);
        GenerateSingle(pos, radius0,angle0,1);
    }
    // generate grasses and flowers
    for (int i=0; i<plantBunch->grassNum; i++) {
        float randomR = GetRandomReal(0.0,1.0);
        float radius0 = FMath::Pow(randomR, plantBunch->grassCenter) * radius;
        float angle0 = GetRandomReal(0.0f, 6.28f);
        GenerateSingle(pos, radius0,angle0,2);
    }
    
    
    
    UE_LOG(LogClass, Log, TEXT("Generate.%s"));
}
