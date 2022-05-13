// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomFilter.h"

#define LOCTEXT_NAMESPACE "CustomFilter"

DEFINE_LOG_CATEGORY(MyLog);

FLinearColor CustomFilter::GetColor() const
{
	if (filterKey.Equals("Color") && filterValue.Equals("Yellow"))
		return FLinearColor::Yellow;
	if (filterKey.Equals("Color") && filterValue.Equals("White"))
		return FLinearColor::White;
	if (filterKey.Equals("Color") && filterValue.Equals("Green"))
		return FLinearColor::Green;
	return FLinearColor::Blue;
}

// �ڲ�������,���ܳ�ͻ
FString CustomFilter::GetName() const
{
	return TEXT("FoliageColorFilter" + filterName);
}

// �༭����ʾ������
FText CustomFilter::GetDisplayName() const
{
	const std::string displayName(TCHAR_TO_UTF8(*filterName));
	const FString s = filterName;
	return FText::FromString(s);
}

// ���ָ���Ƶ�Filter�б���ʱ��ʾ����ʾ
FText CustomFilter::GetToolTipText() const
{
	return LOCTEXT("ColorFilter", "This is a Color Filter");
}


// �ú������������������ʾ���ļ����µ�ÿ���ļ�ִ���жϣ�return true����ʾ��
bool CustomFilter::PassesFilter(FAssetFilterType InItem) const
{
	FName itemName = InItem.GetItemName();
	FString nameString = itemName.ToString();

	//ȥ����uasset��Դ
	FString physicalPath;
	InItem.GetItemPhysicalPath(physicalPath);
	std::string physicalPathSTDString(TCHAR_TO_UTF8(*physicalPath));
	if (!physicalPathSTDString.find(".uasset"))
		return false;
	//UE_LOG(MyLog, Warning, TEXT("ErrCode :%s"), *physicalPath);

	//��ȡ���·��,eg,"/MyRoot/MyFolder/MyFile"
	//�������"/Game/MyGame/Art/Trees/NewPlants/BP_conifer1.BP_conifer1"
	//��Ҫ����ͷ��Game�޸�ΪContent����β��ԭ��׺�ĵ���Ϊʲô���к��ļ���һ���ĺ�׺?������β���� .uaasset
	FString path = InItem.GetVirtualPath().ToString();
	std::string pathSTDString(TCHAR_TO_UTF8(*path));
	std::size_t dotPos = pathSTDString.find_last_of(".");
	pathSTDString = pathSTDString.substr(0, dotPos) + ".uasset";
	pathSTDString = "/Content" + pathSTDString.substr(5, pathSTDString.length() - 5);
	path = "/Content" + path.Right(path.Len() - 5) + ".uasset";
	path = FString(pathSTDString.c_str());

	//UE_LOG(MyLog, Warning, TEXT("filePath:%s"), *path); //debug

	DBApi::getAllTagsByPlantNameRequest request;
	request.plantName = path;
	request.projName = FApp::GetProjectName();

	DBApi::getAllTagsByPlantNameResult result;
	result = DBApi::getAllTagsByPlantName(request);

	//UE_LOG(MyLog, Warning, TEXT("ErrCode :%d"), result.errCode); //debug

	TArray<DBApi::stringKV> stringTags = result.stringTags;
	TArray<DBApi::numberKV> numberTags = result.numberTags;

	for (int32 Index = 0; Index != stringTags.Num(); ++Index)
	{
		if (stringTags[Index].key.Equals(filterKey))
		{
			if (stringTags[Index].value.Equals(filterValue))
			{
				return true;
			}
		}
	}

	return false;
}

#undef LOCTEXT_NAMESPACE