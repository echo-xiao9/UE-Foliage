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

// 内部用名称,不能冲突
FString CustomFilter::GetName() const
{
	return TEXT("FoliageColorFilter" + filterName);
}

// 编辑器显示的名称
FText CustomFilter::GetDisplayName() const
{
	const std::string displayName(TCHAR_TO_UTF8(*filterName));
	const FString s = filterName;
	return FText::FromString(s);
}

// 鼠标指针移到Filter列表上时显示的提示
FText CustomFilter::GetToolTipText() const
{
	return LOCTEXT("ColorFilter", "This is a Color Filter");
}


// 该函数对内容浏览器中显示的文件夹下的每个文件执行判断，return true则显示。
bool CustomFilter::PassesFilter(FAssetFilterType InItem) const
{
	FName itemName = InItem.GetItemName();
	FString nameString = itemName.ToString();

	//去除非uasset资源
	FString physicalPath;
	InItem.GetItemPhysicalPath(physicalPath);
	std::string physicalPathSTDString(TCHAR_TO_UTF8(*physicalPath));
	if (!physicalPathSTDString.find(".uasset"))
		return false;
	//UE_LOG(MyLog, Warning, TEXT("ErrCode :%s"), *physicalPath);

	//获取相对路径,eg,"/MyRoot/MyFolder/MyFile"
	//结果类似"/Game/MyGame/Art/Trees/NewPlants/BP_conifer1.BP_conifer1"
	//需要将开头的Game修改为Content；结尾的原后缀改掉（为什么会有和文件名一样的后缀?）；结尾加上 .uaasset
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