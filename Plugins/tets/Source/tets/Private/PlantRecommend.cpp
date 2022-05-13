// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantRecommend.h"


TArray<FString> UPlantRecommend::GetRecommendSubTrees(FString mainTreeName)
{
	TArray<DBApi::stringKV> stringTags;
	TArray<DBApi::numberKV> numberTags;
	int err = GetPlantTags(mainTreeName, stringTags, numberTags);
	if (err) {
		// error handle
	}

	// special rules
	//DBApi::numberKV* layerKV = numberTags.FindByPredicate([](DBApi::numberKV kv) { return kv.key == "Layer"; });
	//layerKV->value = (float)PlantLayer::SubTree;

	// similarity match
	DBApi::findAllPlantsWithSomeTagsRequest similarRequest;
	similarRequest.projName = FApp::GetProjectName();
	similarRequest.stringTags = stringTags;
	TArray<DBApi::numberKVRequest> numberTagsRange;
	for (DBApi::numberKV kv : numberTags) {
		DBApi::numberKVRequest numberRange;
		numberRange.key = kv.key;
		float sigma = abs(kv.value / 0x10);
		numberRange.lowerBound = kv.value - sigma;
		numberRange.upperBound = kv.value + sigma;
		numberTagsRange.Add(numberRange);
	}
	similarRequest.numberTags = numberTagsRange;
	DBApi::findAllPlantsWithSomeTagsResult similarRes = DBApi::findAllPlantsWithSomeTags(similarRequest);
	if (similarRes.errCode) {
		// error handle
	}
	
	TArray<FString> candidates;
	for (DBApi::plant plant : similarRes.plants) {
		if ((plant.hierarchy & (int)PlantLayer::SubTree) == 0x0) {
			continue;
		}
		candidates.Add(plant.name);
	}

	return candidates;
}

TArray<FString> UPlantRecommend::GetRecommendBushes(FString mainTreeName)
{
	TArray<DBApi::stringKV> stringTags;
	TArray<DBApi::numberKV> numberTags;
	int err = GetPlantTags(mainTreeName, stringTags, numberTags);
	if (err) {
		// error handle
	}

	// similarity match
	DBApi::findAllPlantsWithSomeTagsRequest similarRequest;
	similarRequest.projName = FApp::GetProjectName();
	similarRequest.stringTags = stringTags;
	TArray<DBApi::numberKVRequest> numberTagsRange;
	for (DBApi::numberKV kv : numberTags) {
		DBApi::numberKVRequest numberRange;
		numberRange.key = kv.key;
		float sigma = abs(kv.value / 0x10);
		numberRange.lowerBound = kv.value - sigma;
		numberRange.upperBound = kv.value + sigma;
		numberTagsRange.Add(numberRange);
	}
	similarRequest.numberTags = numberTagsRange;
	DBApi::findAllPlantsWithSomeTagsResult similarRes = DBApi::findAllPlantsWithSomeTags(similarRequest);
	if (similarRes.errCode) {
		// error handle
	}

	TArray<FString> candidates;
	for (DBApi::plant plant : similarRes.plants) {
		if ((plant.hierarchy & (int)PlantLayer::Bush) == 0x0) {
			continue;
		}
		candidates.Add(plant.name);
	}

	return candidates;
}

TArray<FString> UPlantRecommend::GetRecommendGrass(FString mainTreeName)
{
	TArray<DBApi::stringKV> stringTags;
	TArray<DBApi::numberKV> numberTags;
	int err = GetPlantTags(mainTreeName, stringTags, numberTags);
	if (err) {
		// error handle
	}

	// similarity match
	DBApi::findAllPlantsWithSomeTagsRequest similarRequest;
	similarRequest.projName = FApp::GetProjectName();
	similarRequest.stringTags = stringTags;
	TArray<DBApi::numberKVRequest> numberTagsRange;
	for (DBApi::numberKV kv : numberTags) {
		DBApi::numberKVRequest numberRange;
		numberRange.key = kv.key;
		float sigma = abs(kv.value / 0x10);
		numberRange.lowerBound = kv.value - sigma;
		numberRange.upperBound = kv.value + sigma;
		numberTagsRange.Add(numberRange);
	}
	similarRequest.numberTags = numberTagsRange;
	DBApi::findAllPlantsWithSomeTagsResult similarRes = DBApi::findAllPlantsWithSomeTags(similarRequest);
	if (similarRes.errCode) {
		// error handle
	}

	TArray<FString> candidates;
	for (DBApi::plant plant : similarRes.plants) {
		if ((plant.hierarchy & (int)PlantLayer::Grass) == 0x0) {
			continue;
		}
		candidates.Add(plant.name);
	}

	return candidates;
}

int UPlantRecommend::GetPlantTags(
	FString mainTreeName, 
	TArray<DBApi::stringKV>& stringTags, 
	TArray<DBApi::numberKV>& numberTags)
{
	DBApi::getAllTagsByPlantNameRequest request;
	request.projName = FApp::GetProjectName();
	//request.projName = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	request.plantName = mainTreeName;

	DBApi::getAllTagsByPlantNameResult res = DBApi::getAllTagsByPlantName(request);
	stringTags = res.stringTags;
	numberTags = res.numberTags;
	return res.errCode;
}