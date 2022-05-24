// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DBApi.h"
#include "FoliageGeneratorUtils.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlantRecommend.generated.h"

/**
 * 
 */
UCLASS()
class TETS_API UPlantRecommend : public UObject
{
	GENERATED_BODY()

public:
	static TArray<FString> GetRecommendPlants(FString mainTreeName, FoliageGeneratorUtils::PlantLayer targetLayer);

protected:
	static int GetPlantTags(FString mainTreeName, TArray<DBApi::stringKV>& stringTags, TArray<DBApi::numberKV>& numberTags);
	
};
