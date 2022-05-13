// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "CoreMinimal.h"
#include <string>
#include <vector>
/**
 *
 */
class TETS_API DBApi
{
public:
	DBApi();
	~DBApi();
	static std::string getDataFromDatabase(std::string request);
	struct stringKV {
		FString key;
		FString value;
	};

	struct numberKV {
		FString key;
		float value;
	};

	struct numberKVRequest {
		FString key;
		float lowerBound;
		float upperBound;
	};

	enum tagType {
		NUMBER,
		STRING
	};
	struct plant {
		FString name;
		int hierarchy;
	};

	//给定一个植物名称，寻找其全部tag
	USTRUCT()
		struct getAllTagsByPlantNameRequest {
		FString projName;
		FString plantName;

	};
	USTRUCT()
		struct getAllTagsByPlantNameResult {
		int errCode;
		TArray<stringKV> stringTags;
		TArray<numberKV> numberTags;
	};
	static getAllTagsByPlantNameResult getAllTagsByPlantName(getAllTagsByPlantNameRequest request);

	//给定一个植物名称和tag的种类，寻找在给定种类下的所有tag
	USTRUCT()
		struct getOneTagByPlantNameRequest {
		FString projName;
		FString plantName;
		tagType type;
		FString tag;
	};
	USTRUCT()
		struct getOneTagByPlantNameResult {
		int errCode;
		TArray<stringKV> stringTags;
		TArray<numberKV> numberTags;
	};
	static getOneTagByPlantNameResult getOneTagByPlantName(getOneTagByPlantNameRequest request);

	USTRUCT()
		struct findAllPlantsWithSomeTagsRequest {
		FString projName;
		TArray<stringKV> stringTags;
		TArray<numberKVRequest> numberTags;
	};
	USTRUCT()
		struct findAllPlantsWithSomeTagsResult {
		int errCode;
		TArray<plant> plants;
	};

	static findAllPlantsWithSomeTagsResult findAllPlantsWithSomeTags(findAllPlantsWithSomeTagsRequest request);


	USTRUCT()
		struct getAllPlantsInDatabaseRequest {
		FString projName;
	};
	USTRUCT()
		struct getAllPlantsInDatabaseResult {
		int errCode;
		TArray<plant> plants;
	};
	static getAllPlantsInDatabaseResult getAllPlantsInDatabase(getAllPlantsInDatabaseRequest request);

	USTRUCT()
		struct getAllPlantsInOneHierarchyRequest {
		FString projName;
		int hierarchy;
	};
	USTRUCT()
		struct getAllPlantsInOneHierarchyResult {
		int errCode;
		TArray<plant> plants;
	};
	static getAllPlantsInOneHierarchyResult getAllPlantsInOneHierarchy(getAllPlantsInOneHierarchyRequest request);
};

