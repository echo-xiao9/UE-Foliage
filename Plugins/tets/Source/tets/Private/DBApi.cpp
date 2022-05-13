// Fill out your copyright notice in the Description page of Project Settings.


#include "DBApi.h"
#pragma once
#include<iostream>
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/PreWindowsApi.h"
#include<WinSock2.h>
#include<Windows.h>//必须在<WinSock2.h>的下面包含，否则编译不通过
#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
#pragma comment(lib,"WS2_32.lib")//要包含WinSock2.h必须要包这个库
#include<tchar.h>

#include "Json/Public/JSON.h"
#include "Json/Public/Serialization/JsonSerializer.h"
#include "Json/Public/Serialization/JsonReader.h"
using namespace std;

DBApi::DBApi()
{
}

DBApi::~DBApi()
{
}

std::string DBApi::getDataFromDatabase(std::string request)
{
	UE_LOG(LogClass, Warning, TEXT("request is : %s\n"), *(FString(request.c_str())));
	WSADATA v1 = { 0 };
	if (WSAStartup(MAKEWORD(2, 2), &v1) != 0)
	{
		UE_LOG(LogClass, Warning, TEXT("startup error\n"));
		return "error";
	}
	// 创建套节字
	SOCKET CommunicateSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (CommunicateSocket == INVALID_SOCKET)
	{
		UE_LOG(LogClass, Warning, TEXT("Failed socket() \n"));
		printf(" Failed socket() \n");
		return "error";
	}
	// 填写远程地址信息
	sockaddr_in ServerAddress;
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(10086);
	// 注意，这里要填写服务器程序（TCPServer程序）所在机器的IP地址
	// 如果你的计算机没有联网，直接使用127.0.0.1即可
	ServerAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  //192.168.0.106

	if (connect(CommunicateSocket, (sockaddr*)&ServerAddress, sizeof(ServerAddress)) == SOCKET_ERROR)
	{
		UE_LOG(LogClass, Warning, TEXT("Failed connect() \n"));
		printf(" Failed connect() \n");
		return "error";
	}
	send(CommunicateSocket, request.c_str(), request.size(), 0);
	// 接收数据
	char* BufferData = new char[4096];
	int ReturnLength = recv(CommunicateSocket, BufferData, 4096, 0);//receive
	if (ReturnLength > 0)
	{
		if (!(ReturnLength >= 4096))BufferData[ReturnLength] = '\0';  //确保收到的字符串是以/0结尾
		printf(" 接收到数据：%s", BufferData);
		UE_LOG(LogClass, Warning, TEXT("Get Data %s \n"), *(FString(BufferData)));
	}
	std::string res(BufferData);
	delete[] BufferData;
	// 关闭套节字
	closesocket(CommunicateSocket);
	WSACleanup();//与WSAStartup配合使用
	return res;
}

DBApi::getAllTagsByPlantNameResult DBApi::getAllTagsByPlantName(getAllTagsByPlantNameRequest request)
{
	FString requestString;
	TSharedPtr<FJsonObject> requestJson = MakeShareable(new FJsonObject); //TsharedRef对应C++11智能指针
	requestJson->SetStringField("method", "getAllTagsByPlantName");
	requestJson->SetStringField("projName", request.projName);
	requestJson->SetStringField("plantName", request.plantName);
	TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&requestString);
	FJsonSerializer::Serialize(requestJson.ToSharedRef(), Writer);

	UE_LOG(LogClass, Warning, TEXT("%s"), *requestString);
	FString rawRes = FString(getDataFromDatabase(std::string(TCHAR_TO_UTF8(*requestString))).c_str());
	if (rawRes.Equals(FString("error"))) {
		getAllTagsByPlantNameResult result;
		result.errCode = -3;
		return result;
	}
	getAllTagsByPlantNameResult result;
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(rawRes); //JsonString是要解析的字符串
	if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
		result.errCode = JsonObject->GetIntegerField(TEXT("errCode")); //将返回"Mei"
		UE_LOG(LogClass, Warning, TEXT("Raw errCode is %d\n"), result.errCode);
		//其它JSon类型的解析和字符串类似
		if (result.errCode == 0) {
			TArray<TSharedPtr<FJsonValue>> jsonArrayNumber = JsonObject->GetArrayField(TEXT("numberTags"));
			for (auto& str : jsonArrayNumber) {
				auto& tmpObject = str->AsObject();
				DBApi::numberKV tmpKV;
				tmpKV.key = tmpObject->GetStringField(TEXT("key"));
				tmpKV.value = tmpObject->GetNumberField(TEXT("value"));
				if (tmpKV.key.Equals(FString("__useless__")))continue;
				result.numberTags.Add(tmpKV);
			}
			TArray<TSharedPtr<FJsonValue>> jsonArrayString = JsonObject->GetArrayField(TEXT("stringTags"));
			for (auto& str : jsonArrayString) {
				auto& tmpObject = str->AsObject();
				DBApi::stringKV tmpKV;
				tmpKV.key = tmpObject->GetStringField(TEXT("key"));
				tmpKV.value = tmpObject->GetStringField(TEXT("value"));
				if (tmpKV.key.Equals(FString("__useless__")))continue;
				result.stringTags.Add(tmpKV);
			}
		}
	}
	return result;
}

DBApi::getOneTagByPlantNameResult DBApi::getOneTagByPlantName(getOneTagByPlantNameRequest request)
{
	FString requestString;
	TSharedPtr<FJsonObject> requestJson = MakeShareable(new FJsonObject); //TsharedRef对应C++11智能指针
	requestJson->SetStringField("method", "getOneTagByPlantName");
	requestJson->SetStringField("projName", request.projName);
	requestJson->SetStringField("plantName", request.plantName);
	FString typeStr;
	if (request.type == DBApi::NUMBER) {
		typeStr = "number";
	}
	else if (request.type == DBApi::STRING) {
		typeStr = "string";
	}
	requestJson->SetStringField("type", typeStr);
	requestJson->SetStringField("tag", request.tag);
	TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&requestString);
	FJsonSerializer::Serialize(requestJson.ToSharedRef(), Writer);

	FString rawRes = FString(getDataFromDatabase(std::string(TCHAR_TO_UTF8(*requestString))).c_str());
	if (rawRes.Equals(FString("error"))) {
		getOneTagByPlantNameResult result;
		result.errCode = -3;
		return result;
	}
	getOneTagByPlantNameResult result;
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(rawRes); //JsonString是要解析的字符串
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		result.errCode = JsonObject->GetIntegerField(TEXT("errCode")); //将返回"Mei"
		//其它JSon类型的解析和字符串类似
		if (result.errCode == 0) {
			TArray<TSharedPtr<FJsonValue>> jsonArrayNumber = JsonObject->GetArrayField(TEXT("numberTags"));
			for (auto& str : jsonArrayNumber) {
				auto& tmpObject = str->AsObject();
				DBApi::numberKV tmpKV;
				tmpKV.key = tmpObject->GetStringField(TEXT("key"));
				tmpKV.value = tmpObject->GetNumberField(TEXT("value"));
				if (tmpKV.key.Equals(FString("__useless__")))continue;
				result.numberTags.Add(tmpKV);
			}
			TArray<TSharedPtr<FJsonValue>> jsonArrayString = JsonObject->GetArrayField(TEXT("stringTags"));
			for (auto& str : jsonArrayString) {
				auto& tmpObject = str->AsObject();
				DBApi::stringKV tmpKV;
				tmpKV.key = tmpObject->GetStringField(TEXT("key"));
				tmpKV.value = tmpObject->GetStringField(TEXT("value"));
				if (tmpKV.key.Equals(FString("__useless__")))continue;
				result.stringTags.Add(tmpKV);
			}
		}
	}
	return result;
}

DBApi::findAllPlantsWithSomeTagsResult DBApi::findAllPlantsWithSomeTags(findAllPlantsWithSomeTagsRequest request)
{
	FString requestString;
	TSharedPtr<FJsonObject> requestJson = MakeShareable(new FJsonObject); //TsharedRef对应C++11智能指针
	requestJson->SetStringField("method", "findAllPlantsWithSomeTags");
	requestJson->SetStringField("projName", request.projName);
	TArray<TSharedPtr<FJsonValue>> numberTagNames;
	for (auto& tag : request.numberTags) {
		TSharedPtr<FJsonObject> tmpObj = MakeShareable(new FJsonObject);
		tmpObj->SetStringField("key", tag.key);
		tmpObj->SetNumberField("lowerBound", tag.lowerBound);
		tmpObj->SetNumberField("upperBound", tag.upperBound);
		TSharedPtr<FJsonValue> tagPtr = MakeShareable(new FJsonValueObject(tmpObj));
		numberTagNames.Add(tagPtr);
	}
	requestJson->SetArrayField("numberTags", numberTagNames);

	TArray<TSharedPtr<FJsonValue>> stringTagNames;
	for (auto& tag : request.stringTags) {
		TSharedPtr<FJsonObject> tmpObj = MakeShareable(new FJsonObject);
		tmpObj->SetStringField("key", tag.key);
		tmpObj->SetStringField("value", tag.value);
		TSharedPtr<FJsonValue> tagPtr = MakeShareable(new FJsonValueObject(tmpObj));
		stringTagNames.Add(tagPtr);
	}
	requestJson->SetArrayField("stringTags", stringTagNames);
	TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&requestString);
	FJsonSerializer::Serialize(requestJson.ToSharedRef(), Writer);


	FString rawRes = FString(getDataFromDatabase(std::string(TCHAR_TO_UTF8(*requestString))).c_str());
	if (rawRes.Equals(FString("error"))) {
		findAllPlantsWithSomeTagsResult result;
		result.errCode = -3;
		return result;
	}
	findAllPlantsWithSomeTagsResult result;
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(rawRes); //JsonString是要解析的字符串
	if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
		result.errCode = JsonObject->GetIntegerField(TEXT("errCode")); //将返回"Mei"
		//其它JSon类型的解析和字符串类似
		if (result.errCode == 0) {
			auto& arrayRes = JsonObject->GetArrayField(TEXT("plants"));
			for (auto& plant : arrayRes) {
				auto& obj = plant->AsObject();
				DBApi::plant onePlant;
				onePlant.name = obj->GetStringField(TEXT("name"));
				onePlant.hierarchy = obj->GetIntegerField(TEXT("hierarchy"));
				if (onePlant.name.Equals(FString("__useless__")))continue;
				result.plants.Add(onePlant);
			}
		}
	}
	return result;
}

DBApi::getAllPlantsInDatabaseResult DBApi::getAllPlantsInDatabase(getAllPlantsInDatabaseRequest request)
{
	FString requestString;
	TSharedPtr<FJsonObject> requestJson = MakeShareable(new FJsonObject); //TsharedRef对应C++11智能指针
	requestJson->SetStringField("method", "getAllPlantsInDatabase");
	requestJson->SetStringField("projName", request.projName);
	TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&requestString);
	FJsonSerializer::Serialize(requestJson.ToSharedRef(), Writer);
	UE_LOG(LogClass, Warning, TEXT("%s\n"), *requestString);
	FString rawRes = FString(getDataFromDatabase(std::string(TCHAR_TO_UTF8(*requestString))).c_str());
	if (rawRes.Equals(FString("error"))) {
		getAllPlantsInDatabaseResult result;
		result.errCode = -3;
		return result;
	}
	getAllPlantsInDatabaseResult result;
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(rawRes); //JsonString是要解析的字符串
	if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
		result.errCode = JsonObject->GetIntegerField(TEXT("errCode")); //将返回"Mei"
		//其它JSon类型的解析和字符串类似
		if (result.errCode == 0) {
			auto& arrayRes = JsonObject->GetArrayField(TEXT("plants"));
			for (auto& plant : arrayRes) {
				auto& obj = plant->AsObject();
				DBApi::plant onePlant;
				onePlant.name = obj->GetStringField(TEXT("name"));
				onePlant.hierarchy = obj->GetIntegerField(TEXT("hierarchy"));
				if (onePlant.name.Equals(FString("__useless__")))continue;
				result.plants.Add(onePlant);
			}
		}
	}
	return result;
}

DBApi::getAllPlantsInOneHierarchyResult DBApi::getAllPlantsInOneHierarchy(getAllPlantsInOneHierarchyRequest request)
{
	FString requestString;
	TSharedPtr<FJsonObject> requestJson = MakeShareable(new FJsonObject); //TsharedRef对应C++11智能指针
	requestJson->SetStringField("method", "getAllPlantsInOneHierarchy");
	requestJson->SetStringField("projName", request.projName);
	requestJson->SetNumberField("hierarchy", request.hierarchy);
	TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&requestString);
	FJsonSerializer::Serialize(requestJson.ToSharedRef(), Writer);
	UE_LOG(LogClass, Warning, TEXT("%s\n"), *requestString);
	FString rawRes = FString(getDataFromDatabase(std::string(TCHAR_TO_UTF8(*requestString))).c_str());
	if (rawRes.Equals(FString("error"))) {
		getAllPlantsInOneHierarchyResult result;
		result.errCode = -3;
		return result;
	}
	getAllPlantsInOneHierarchyResult result;
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(rawRes); //JsonString是要解析的字符串
	if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
		result.errCode = JsonObject->GetIntegerField(TEXT("errCode")); //将返回"Mei"
		//其它JSon类型的解析和字符串类似
		if (result.errCode == 0) {
			auto& arrayRes = JsonObject->GetArrayField(TEXT("plants"));
			for (auto& plant : arrayRes) {
				auto& obj = plant->AsObject();
				DBApi::plant onePlant;
				onePlant.name = obj->GetStringField(TEXT("name"));
				onePlant.hierarchy = obj->GetIntegerField(TEXT("hierarchy"));
				if (onePlant.name.Equals(FString("__useless__")))continue;
				result.plants.Add(onePlant);
			}
		}
	}
	return result;
}
