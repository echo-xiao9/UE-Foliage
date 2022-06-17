// Fill out your copyright notice in the Description page of Project Settings.
// this is for plantBunch detail panel customization
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlantRecommend.h"
#include "PlantBunch.generated.h"
/**
 *
 */


UENUM()
enum class EcoSystem
{
    Subtropical = 1,
    Desert = 2,
    Coniferous = 3,
    Tropical = 4,
 
};


UCLASS()
class TETS_API UPlantBunch: public UDataAsset
{
    
    GENERATED_BODY()
private:
    
    
    
public:

    UPROPERTY(EditAnywhere, Category = "Settings")
    float radius;
    
    // Central aggregation of associated plants
    UPROPERTY(EditAnywhere, Category = "Settings")
    int center;

    // this is for hard code choose of mainTree
    UPROPERTY(EditAnywhere, Category = "Settings")
    EcoSystem ecoSystem;

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
    
    void recommendPlant();

   
    
};
