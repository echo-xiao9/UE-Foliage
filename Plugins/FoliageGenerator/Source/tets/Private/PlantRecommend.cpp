// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantRecommend.h"


TArray<FString> UPlantRecommend::GetRecommendPlants(
	FString mainTreeName, 
	FoliageGeneratorUtils::PlantLayer targetLayer,
	FString specificEcosystems)
{
	TArray<DBApi::stringKV> stringTags;
	TArray<DBApi::numberKV> numberTags;
	int err = GetPlantTags(mainTreeName, stringTags, numberTags);
	if (err) {
		// error handle
	}

	// phase ecosystem
	TArray<FString> ecosytems;
	if (specificEcosystems == "") {
		DBApi::stringKV* ecosysKV = stringTags.FindByPredicate([](DBApi::stringKV kv) { return kv.key == "Ecosystem"; });
		if (!ecosysKV) {
			// ecosystem tag missing
		}
		specificEcosystems = ecosysKV->value;
	}
	FString ecosysPop;
	while (specificEcosystems.Split("/", &ecosysPop, &specificEcosystems)) {
		ecosytems.Add(ecosysPop);
	}
	ecosytems.Add(specificEcosystems);

	// color match
	DBApi::findAllPlantsWithSomeTagsRequest colorRequest;
	colorRequest.projName = FApp::GetProjectName();
	TArray<DBApi::numberKVRequest> colorRanges;
	DBApi::numberKV* colorH = numberTags.FindByPredicate([](DBApi::numberKV kv) { return kv.key == "Color_H"; });
	DBApi::numberKV* colorS = numberTags.FindByPredicate([](DBApi::numberKV kv) { return kv.key == "Color_S"; });
	DBApi::numberKV* colorV = numberTags.FindByPredicate([](DBApi::numberKV kv) { return kv.key == "Color_V"; });
	if (!colorH || !colorS || !colorV) {
		// color tag missing
	}

	DBApi::numberKVRequest colorRange;

	float HOff = 2.0;
	colorRange.key = colorH->key;
	colorRange.lowerBound = colorH->value - 360.0 * HOff / (colorS->value + HOff);
	colorRange.upperBound = colorH->value + 360.0 * HOff / (colorS->value + HOff);
	if (targetLayer == FoliageGeneratorUtils::PlantLayer::Flower || targetLayer == FoliageGeneratorUtils::PlantLayer::Stone) {
		colorRange.lowerBound = 0;
		colorRange.upperBound = 360;
	}
	colorRanges.Add(colorRange);

	float SOff = 20.0;
	colorRange.key = colorS->key;
	colorRange.lowerBound = colorS->value - SOff;
	colorRange.upperBound = colorS->value + SOff;
	if (targetLayer == FoliageGeneratorUtils::PlantLayer::Stone) {
		colorRange.lowerBound = 0;
		colorRange.upperBound = 100;
	}
	colorRanges.Add(colorRange);

	float VOff = 50.0;
	colorRange.key = colorV->key;
	colorRange.lowerBound = colorV->value - VOff;
	colorRange.upperBound = colorV->value + VOff;
	if (targetLayer == FoliageGeneratorUtils::PlantLayer::Stone) {
		colorRange.lowerBound = 0;
		colorRange.upperBound = 100;
	}
	colorRanges.Add(colorRange);
	colorRequest.numberTags = colorRanges;
	DBApi::findAllPlantsWithSomeTagsResult colorRes = DBApi::findAllPlantsWithSomeTags(colorRequest);
	if (colorRes.errCode) {
		// error handle
	}

	// ecosystem & layer filter
	TArray<FString> candidates;
	for (DBApi::plant plant : colorRes.plants) {
		if (plant.hierarchy != (int)targetLayer) {
			continue;
		}

		TArray<DBApi::stringKV> strTags;
		TArray<DBApi::numberKV> numTags;
		int errCode = GetPlantTags(plant.name, strTags, numTags);
		if (errCode) {
			// error handle
		}

		// phase ecosystem
		bool ecoMatch = false;
		DBApi::stringKV* ecoKV = strTags.FindByPredicate([](DBApi::stringKV kv) { return kv.key == "Ecosystem"; });
		if (!ecoKV) {
			// ecosystem tag missing
		}
		FString ecoPop, ecoVal = ecoKV->value;
		while (ecoVal.Split("/", &ecoPop, &ecoVal)) {
			if (ecosytems.Contains(ecoPop)) {
				ecoMatch = true;
				break;
			}
		}
		ecoMatch |= ecosytems.Contains(ecoVal);
		if (!ecoMatch) {
			continue;
		}

		candidates.Add(plant.name);
	}

	// similarity match
	/*
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
	*/

	// sort?

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

TArray<UBlueprint*> UPlantRecommend::GetRecommendPlantsBP(
	FString mainTreeName,
	FoliageGeneratorUtils::PlantLayer targetLayer,
	FString specificEcosystems)
{
	TArray<FString> names = GetRecommendPlants(mainTreeName, targetLayer, specificEcosystems);
	TArray<UBlueprint*> BPs;
	for (int i = 0; i < names.Num(); ++i) {
		FString tmp;
		names[i].Split(".uasset", &names[i], &tmp, ESearchCase::CaseSensitive, ESearchDir::FromEnd);
		names[i].Split("/", &names[i], &tmp, ESearchCase::CaseSensitive, ESearchDir::FromEnd);
		FString path(TEXT("Blueprint'/Game" + names[i] + "/" + tmp + "." + tmp + "'"));
		FSoftObjectPath assetRef(path);
		TAssetPtr<UBlueprint> assetPtr(assetRef);
		UBlueprint* bp = assetPtr.LoadSynchronous();
		BPs.Add(bp);
	}
	return BPs;
}