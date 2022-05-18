// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlantBunch.generated.h"
/**
 *
 */

UCLASS()
class TETS_API UPlantBunch: public UDataAsset
{
    
    GENERATED_BODY()
    
public:
    
    UPROPERTY(EditAnywhere, Category = "settings")
    float radius;
    
    UPROPERTY(EditAnywhere, Category = "settings")
    FString color;
    
    UPROPERTY(EditAnywhere, Category = "settings")
    int center;

    
    UPROPERTY(EditAnywhere, Category = "MainTree")
    UBlueprint* mainTree;
    UPROPERTY(EditAnywhere, Category = "MainTree")
    FString mainTag;
    
    UPROPERTY(EditAnywhere, Category = "SubTrees")
    TArray<UBlueprint*> subTrees;
    UPROPERTY(EditAnywhere, Category = "SubTrees")
    int subTreeNum;
    
    UPROPERTY(EditAnywhere, Category = "Bushes")
    TArray<UBlueprint*> bushes;
    UPROPERTY(EditAnywhere, Category = "Bushes")
    int bushNum;
    
    UPROPERTY(EditAnywhere, Category = "Grasses")
    TArray<UBlueprint*> grasses;
    
    UPROPERTY(EditAnywhere, Category = "Grasses")
    int grassNum;
    
    UPROPERTY(EditAnywhere, Category = "Flowers")
    TArray<UBlueprint*> flowers;
    
    UPROPERTY(EditAnywhere, Category = "Flowers")
    int flowerNum;
    
    UPROPERTY(EditAnywhere, Category = "Stones")
    TArray<UBlueprint*> stones;
    
    UPROPERTY(EditAnywhere, Category = "Stones")
    int stoneNum;
    
    
    
   
    
};
