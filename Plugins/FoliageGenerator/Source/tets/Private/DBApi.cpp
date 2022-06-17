// Fill out your copyright notice in the Description page of Project Settings.


#include "DBApi.h"
//#pragma once
#include<iostream>

#ifdef __APPLE__
    #include <stdio.h>
    #include <string.h>
    #include <unistd.h>
    #include <ctype.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <stdlib.h>
    #include <errno.h>
    #include <string>
#else
    #include "Windows/AllowWindowsPlatformTypes.h"
    #include "Windows/PreWindowsApi.h"
    #include <WinSock2.h>
    #include <Windows.h>//������<WinSock2.h>�����������������벻ͨ��
    #include "Windows/PostWindowsApi.h"
    #include "Windows/HideWindowsPlatformTypes.h"
    #pragma comment(lib,"WS2_32.lib")//Ҫ����WinSock2.h����Ҫ�������
#endif

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


#ifdef __APPLE__
std::string getDataFromSocket(std::string request){
    char ip[20]={"127.0.0.1"};
    int port=10086;
    int server_fd;
    struct sockaddr_in server_listen_addr;
    bzero(&server_listen_addr,sizeof(server_listen_addr));
    server_listen_addr.sin_family=AF_INET;
    server_listen_addr.sin_port=htons(port);
    inet_pton(AF_INET,"127.0.0.1",(void*)&server_listen_addr.sin_addr);
    // inet_aton(ip,&server_listen_addr.sin_addr);
    // bzero(&(server_listen_addr.sin_zero),8);
    server_fd=socket(AF_INET,SOCK_STREAM,0);
    int ret=connect(server_fd,(const struct sockaddr *)&server_listen_addr,
                    sizeof(struct sockaddr));
    if(ret<0)
    {
        perror("error: socket connect!");
        exit(1);
    }
    
    int num=send(server_fd,(void*)(request.c_str()),request.size(),0);
    char* buffer = new char[1024 * 1024];
    socklen_t len = sizeof(struct sockaddr);
    ssize_t rb = recv(server_fd,buffer,1024 * 1024 - 1, 0);
    buffer[rb / sizeof(buffer[0])] = '\0';
    close(server_fd);
    std::string res(buffer);
    delete[] buffer;
    return res;
}

#else
std::string DBApi::getDataFromDatabase(std::string request)
{
    UE_LOG(LogClass, Warning, TEXT("request is : %s\n"), *(FString(request.c_str())));
    WSADATA v1 = { 0 };
    if (WSAStartup(MAKEWORD(2, 2), &v1) != 0)
    {
        UE_LOG(LogClass, Warning, TEXT("startup error\n"));
        return "error";
    }
    // �����׽���
    SOCKET CommunicateSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (CommunicateSocket == INVALID_SOCKET)
    {
        UE_LOG(LogClass, Warning, TEXT("Failed socket() \n"));
        printf(" Failed socket() \n");
        return "error";
    }
    // ��дԶ�̵�ַ��Ϣ
    sockaddr_in ServerAddress;
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(10086);
    // ע�⣬����Ҫ��д����������TCPServer�������ڻ�����IP��ַ
    // �����ļ����û��������ֱ��ʹ��127.0.0.1����
    ServerAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  //192.168.0.106
    
    if (connect(CommunicateSocket, (sockaddr*)&ServerAddress, sizeof(ServerAddress)) == SOCKET_ERROR)
    {
        UE_LOG(LogClass, Warning, TEXT("Failed connect() \n"));
        printf(" Failed connect() \n");
        return "error";
    }
    send(CommunicateSocket, request.c_str(), request.size(), 0);
    // ��������
    char* BufferData = new char[1024 * 1024];
    int ReturnLength = recv(CommunicateSocket, BufferData, 1024 * 1024, 0);//receive
    if (ReturnLength > 0)
    {
        if (!(ReturnLength >= 1024 * 1024))BufferData[ReturnLength] = '\0';  //ȷ���յ����ַ�������/0��β
        printf(" ���յ����ݣ�%s", BufferData);
        UE_LOG(LogClass, Warning, TEXT("Get Data %s \n"), *(FString(BufferData)));
    }
    std::string res(BufferData);
    delete[] BufferData;
    // �ر��׽���
    closesocket(CommunicateSocket);
    WSACleanup();//��WSAStartup���ʹ��
    return res;
}
#endif

DBApi::getAllTagsByPlantNameResult DBApi::getAllTagsByPlantName(getAllTagsByPlantNameRequest request)
{
    FString requestString;
    TSharedPtr<FJsonObject> requestJson = MakeShareable(new FJsonObject); //TsharedRef��ӦC++11����ָ��
    requestJson->SetStringField("method", "getAllTagsByPlantName");
    requestJson->SetStringField("projName", request.projName);
    requestJson->SetStringField("plantName", request.plantName);
    TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&requestString);
    FJsonSerializer::Serialize(requestJson.ToSharedRef(), Writer);
    
    UE_LOG(LogClass, Warning, TEXT("%s"), *requestString);


#ifdef __APPLE__
    FString rawRes = FString(getDataFromSocket(std::string(TCHAR_TO_UTF8(*requestString))).c_str());
#else
    FString rawRes = FString(getDataFromDatabase(std::string(TCHAR_TO_UTF8(*requestString))).c_str());
#endif
    
    if (rawRes.Equals(FString("error"))) {
        getAllTagsByPlantNameResult result;
        result.errCode = -3;
        return result;
    }
    getAllTagsByPlantNameResult result;
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(rawRes); //JsonString��Ҫ�������ַ���
    if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
        result.errCode = JsonObject->GetIntegerField(TEXT("errCode")); //������"Mei"
        UE_LOG(LogClass, Warning, TEXT("Raw errCode is %d\n"), result.errCode);
        //����JSon���͵Ľ������ַ�������
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
    TSharedPtr<FJsonObject> requestJson = MakeShareable(new FJsonObject); //TsharedRef��ӦC++11����ָ��
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


#ifdef __APPLE__
    FString rawRes = FString(getDataFromSocket(std::string(TCHAR_TO_UTF8(*requestString))).c_str());
#else
    FString rawRes = FString(getDataFromDatabase(std::string(TCHAR_TO_UTF8(*requestString))).c_str());
#endif
    
    if (rawRes.Equals(FString("error"))) {
        getOneTagByPlantNameResult result;
        result.errCode = -3;
        return result;
    }
    getOneTagByPlantNameResult result;
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(rawRes); //JsonString��Ҫ�������ַ���
    if (FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        result.errCode = JsonObject->GetIntegerField(TEXT("errCode")); //������"Mei"
        //����JSon���͵Ľ������ַ�������
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
    TSharedPtr<FJsonObject> requestJson = MakeShareable(new FJsonObject); //TsharedRef��ӦC++11����ָ��
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
    

#ifdef __APPLE__
    FString rawRes = FString(getDataFromSocket(std::string(TCHAR_TO_UTF8(*requestString))).c_str());
#else
    FString rawRes = FString(getDataFromDatabase(std::string(TCHAR_TO_UTF8(*requestString))).c_str());
#endif
    
    if (rawRes.Equals(FString("error"))) {
        findAllPlantsWithSomeTagsResult result;
        result.errCode = -3;
        return result;
    }
    findAllPlantsWithSomeTagsResult result;
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(rawRes); //JsonString��Ҫ�������ַ���
    if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
        result.errCode = JsonObject->GetIntegerField(TEXT("errCode")); //������"Mei"
        //����JSon���͵Ľ������ַ�������
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
    TSharedPtr<FJsonObject> requestJson = MakeShareable(new FJsonObject); //TsharedRef��ӦC++11����ָ��
    requestJson->SetStringField("method", "getAllPlantsInDatabase");
    requestJson->SetStringField("projName", request.projName);
    TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&requestString);
    FJsonSerializer::Serialize(requestJson.ToSharedRef(), Writer);
    UE_LOG(LogClass, Warning, TEXT("%s\n"), *requestString);
#ifdef __APPLE__
    FString rawRes = FString(getDataFromSocket(std::string(TCHAR_TO_UTF8(*requestString))).c_str());
    
#else
    FString rawRes = FString(getDataFromDatabase(std::string(TCHAR_TO_UTF8(*requestString))).c_str());
#endif
    if (rawRes.Equals(FString("error"))) {
        getAllPlantsInDatabaseResult result;
        result.errCode = -3;
        return result;
    }
    getAllPlantsInDatabaseResult result;
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(rawRes); //JsonString��Ҫ�������ַ���
    if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
        result.errCode = JsonObject->GetIntegerField(TEXT("errCode")); //������"Mei"
        //����JSon���͵Ľ������ַ�������
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
    TSharedPtr<FJsonObject> requestJson = MakeShareable(new FJsonObject); //TsharedRef��ӦC++11����ָ��
    requestJson->SetStringField("method", "getAllPlantsInOneHierarchy");
    requestJson->SetStringField("projName", request.projName);
    requestJson->SetNumberField("hierarchy", request.hierarchy);
    TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&requestString);
    FJsonSerializer::Serialize(requestJson.ToSharedRef(), Writer);
    UE_LOG(LogClass, Warning, TEXT("%s\n"), *requestString);

#ifdef __APPLE__
    FString rawRes = FString(getDataFromSocket(std::string(TCHAR_TO_UTF8(*requestString))).c_str());
#else
    FString rawRes = FString(getDataFromDatabase(std::string(TCHAR_TO_UTF8(*requestString))).c_str());
#endif
    if (rawRes.Equals(FString("error"))) {
        getAllPlantsInOneHierarchyResult result;
        result.errCode = -3;
        return result;
    }
    getAllPlantsInOneHierarchyResult result;
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(rawRes); //JsonString��Ҫ�������ַ���
    if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
        result.errCode = JsonObject->GetIntegerField(TEXT("errCode")); //������"Mei"
        //����JSon���͵Ľ������ַ�������
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
