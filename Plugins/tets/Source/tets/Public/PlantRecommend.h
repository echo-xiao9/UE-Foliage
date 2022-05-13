// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DBApi.h"
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
	enum class PlantLayer { MainTree = 0x1, SubTree = 0x2, Bush = 0x4, Grass = 0x8 };

	static TArray<FString> GetRecommendSubTrees(FString mainTreeName);
	static TArray<FString> GetRecommendBushes(FString mainTreeName);
	static TArray<FString> GetRecommendGrass(FString mainTreeName);

protected:
	static int GetPlantTags(FString mainTreeName, TArray<DBApi::stringKV>& stringTags, TArray<DBApi::numberKV>& numberTags);
	
};
