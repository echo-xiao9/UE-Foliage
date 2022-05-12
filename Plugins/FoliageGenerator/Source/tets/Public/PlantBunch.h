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
    int subTreeNum;
    UPROPERTY(EditAnywhere, Category = "settings")
    float radius;
    
    UPROPERTY(EditAnywhere, Category = "settings")
    FString color;

    
    UPROPERTY(EditAnywhere, Category = "MainTree")
    UBlueprint* mainTree;
    UPROPERTY(EditAnywhere, Category = "MainTree")
    FString mainTag;
    
    UPROPERTY(EditAnywhere, Category = "SubTree")
    TArray<UBlueprint*> subTrees;
    
    UPROPERTY(EditAnywhere, Category = "Bushes")
    TArray<UBlueprint*> bushes;
    UPROPERTY(EditAnywhere, Category = "Bushes")
    int bushNum;
    
    UPROPERTY(EditAnywhere, Category = "grass and flowers")
    TArray<UBlueprint*> grassFlowers;
    
    UPROPERTY(EditAnywhere, Category = "grass and flowers")
    int grassNum;
    UPROPERTY(EditAnywhere, Category = "grass and flowers")
    int grassCenter;
    
};
