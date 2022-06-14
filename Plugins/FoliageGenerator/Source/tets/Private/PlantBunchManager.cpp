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

    // adjust the z position according to the landscape
    bool validPos = true;
    float deltaZ = GetLandscapeHeight(newPos, 200, 2000, validPos);
    if (!validPos)
        return;
    newPos = newPos + FVector(0, 0, deltaZ-newPos.Z);
    UE_LOG(LogClass, Log, TEXT("generate sub0 deltaZ:%f"), newPos.Z);

    FTransform spawnLocation0(newPos);
    switch (type) {
        case 0:{
            // sub tree
            int chosenTree = getRandomInt(0,plantBunch->subTrees.Num()-1);
            GetWorld()->SpawnActor<AActor>(plantBunch->subTrees[chosenTree]->GeneratedClass, spawnLocation0);
            break;
        }
        case 1:{
            // bush
            int chosenBush = getRandomInt(0,plantBunch->bushes.Num()-1);
            GetWorld()->SpawnActor<AActor>(plantBunch->bushes[chosenBush]->GeneratedClass, spawnLocation0);
            break;
        }
        case 2:{
            // grass
            int chosenGrass = getRandomInt(0,plantBunch->grasses.Num()-1);
            GetWorld()->SpawnActor<AActor>(plantBunch->grasses[chosenGrass]->GeneratedClass, spawnLocation0);
            break;
        }
        case 3:{
            // flower
            int chosenFlower = getRandomInt(0,plantBunch->flowers.Num()-1);
            GetWorld()->SpawnActor<AActor>(plantBunch->flowers[chosenFlower]->GeneratedClass, spawnLocation0);
            break;
        }
        case 4:{
            // stone
            int chosenStone = getRandomInt(0,plantBunch->stones.Num()-1);
            GetWorld()->SpawnActor<AActor>(plantBunch->stones[chosenStone]->GeneratedClass, spawnLocation0);
            break;
        }
        default:
            break;
    }
    
//    UE_LOG(LogClass, Log, TEXT("chosenTree:%d"), chosenTree);
    
    
}

float APlantBunchManager::GetLandscapeHeight(FVector spawnLocation, float radius, float maxDeltaHeight, bool& validPos)
{
    TArray<FVector> testPositions;
    testPositions.Add(spawnLocation);
    for (int i = 0; i < 8; i++)
    {
        float degree = 45 * i;
        float radian = FMath::DegreesToRadians(degree);
        float SinValue = FMath::Sin(radian);
        float CosValue = FMath::Cos(radian);
        FVector tmpVector1(spawnLocation.X + radius * CosValue, spawnLocation.Y + radius * SinValue, spawnLocation.Z);
        FVector tmpVector2(spawnLocation.X + radius * CosValue / 2, spawnLocation.Y + radius * SinValue / 2, spawnLocation.Z);
        testPositions.Add(tmpVector1);
        testPositions.Add(tmpVector2);
    }

    FVector up(0, 0, 10000);
    float maxZ = 0;
    float minZ = 0;
    bool hasValidZ = false;

    for (const FVector& position : testPositions)
    {
        FVector startPos = position + up;
        FVector endPos = position - up;
        TArray<FHitResult> hits;
        FCollisionShape sphereShape;
        sphereShape.SetSphere(1);

        GetWorld()->SweepMultiByObjectType(hits, startPos, endPos, FQuat::Identity, FCollisionObjectQueryParams(ECC_WorldStatic), sphereShape);

        for (const FHitResult& hit : hits)
        {
            const AActor* hitActor = hit.GetActor();
            if (hitActor->IsA(ALandscape::StaticClass()))
            {
                float z = hit.ImpactPoint.Z;
                if (!hasValidZ)
                {
                    maxZ = z;
                    minZ = z;
                    hasValidZ = true;
                }
                else
                {
                    maxZ = maxZ > z ? maxZ : z;
                    minZ = minZ < z ? minZ : z;
                }
                continue;
            }
        }
    }
    if (maxZ - minZ > maxDeltaHeight)
    {
        UE_LOG(LogClass, Log, TEXT("not smooth ladscape"));
        validPos = false;
        return 0;
    }
    else if (hasValidZ)
    {
        validPos = true;
        return minZ;
    }
    else
    {
        validPos = false;
        UE_LOG(LogClass, Log, TEXT("Can't find Landscape at the position where the foliage is spawned, so the height is set to 0"));
        return 0;
    }
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
    // generate grasses
    for (int i=0; i<plantBunch->grassNum; i++) {
        float randomR = GetRandomReal(0.0,1.0);
        float radius0 = FMath::Pow(randomR, plantBunch->center) * radius;
        float angle0 = GetRandomReal(0.0f, 6.28f);
        GenerateSingle(pos, radius0,angle0,2);
    }
    // generate flowers
    for (int i=0; i<plantBunch->flowerNum; i++) {
        float randomR = GetRandomReal(0.0,1.0);
        float radius0 = FMath::Pow(randomR, plantBunch->center) * radius;
        float angle0 = GetRandomReal(0.0f, 6.28f);
        GenerateSingle(pos, radius0,angle0,3);
    }
    // generate stones
    for (int i=0; i<plantBunch->stoneNum; i++) {
        float randomR = GetRandomReal(0.0,1.0);
        float radius0 = FMath::Pow(randomR, plantBunch->center) * radius;
        float angle0 = GetRandomReal(0.0f, 6.28f);
        GenerateSingle(pos, radius0,angle0,4);
    }
    
    
    
    UE_LOG(LogClass, Log, TEXT("Generate.%s"));
}

