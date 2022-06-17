// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DBApi.h"
#include "FoliageGeneratorUtils.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlantRecommend.generated.h"

/// <summary>
/// Plant recommand class: 
/// provide public static functions for recommandation genarating
/// </summary>
UCLASS()
class TETS_API UPlantRecommend : public UObject
{
    GENERATED_BODY()

public:
    /// <summary>
    /// The base function of recommand,
    /// with specific main tree, layer (and ecosystem).
    /// </summary>
    /// <param name="mainTreeName">the name of main tree in target plant bunch</param>
    /// <param name="targetLayer">the target layer in the plant bunch</param>
    /// <param name="specificEcosystems">
    /// (optional) specific one or more target ecosystems,
    /// if omitted, use the ecosystem of main tree
    /// </param>
    /// <returns>An array of plant names recommanded</returns>
    static TArray<FString> GetRecommendPlants(
        FString mainTreeName,
        FoliageGeneratorUtils::PlantLayer targetLayer,
        FString specificEcosystems = "");

    /// <summary>
    /// A packed function of recommand, return the BPs of plant rathar than names,
    /// with specific main tree, layer (and ecosystem).
    /// </summary>
    /// <param name="mainTreeName">the name of main tree in target plant bunch</param>
    /// <param name="targetLayer">the target layer in the plant bunch</param>
    /// <param name="specificEcosystems">
    /// (optional) specific one or more target ecosystems,
    /// if omitted, use the ecosystem of main tree
    /// </param>
    /// <returns>An array of plant BPs recommanded</returns>
    static TArray<UBlueprint *> GetRecommendPlantsBP(
        FString mainTreeName,
        FoliageGeneratorUtils::PlantLayer targetLayer,
        FString specificEcosystems = "");

protected:
    static int GetPlantTags(FString mainTreeName, TArray<DBApi::stringKV>& stringTags, TArray<DBApi::numberKV>& numberTags);
    
};
