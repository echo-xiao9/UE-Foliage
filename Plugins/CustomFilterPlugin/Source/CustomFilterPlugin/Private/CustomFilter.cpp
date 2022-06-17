// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomFilter.h"

#define LOCTEXT_NAMESPACE "CustomFilter"

// 过滤器的颜色
FLinearColor CustomFilter::GetColor() const
{
	//如果是颜色tag，则按照h值生成过滤器tag颜色
	if (filterType.Equals("Color"))
	{
		float min = FCString::Atof(*filterMinValue);
		float max = FCString::Atof(*filterMaxValue);
		float mid = (min + max) / 2.0;
		FLinearColor hsv(mid, 1, 255, 255);
		FLinearColor rgb = hsv.HSVToLinearRGB();
		//UE_LOG(MyLog, Warning, TEXT("RGB:%f %f %f"), rgb.R, rgb.G, rgb.B); //debug
		rgb = FLinearColor(rgb.R / 255, rgb.G / 255, rgb.B / 255, 255);
		return rgb;
	}
	else
	{
		//用filterName作为参数生成一个颜色值。
		int r=0, g=0, b=0;
		for (int i = 0; i < filterName.Len(); i++)
		{
			r += filterName[i];
			if (i < filterName.Len() / 2)
			{
				g += filterName[i] * 2;
			}	
			else if (i >= filterName.Len() / 2)
			{
				b += filterName[i] * 2;
			}
		}
		r %= 256;
		g %= 256;
		b %= 256;
		FLinearColor color(r / 255.0, g / 255.0, b / 255.0, 255);
		return color;
	}
	
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
	return LOCTEXT("Filter", "This is a custom filter");
}


// 该函数对内容浏览器中显示的文件夹下的每个文件执行判断，return true则说明该文件符合过滤条件。
// 通过文件名和项目名，调用getOneTagByPlantName方法，从数据库获取其tag，并根据过滤器类型进行判断。
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
	//需要将开头的Game去掉；结尾的原后缀改掉（为什么会有和文件名一样的后缀?）；结尾加上 .uaasset
	FString path = InItem.GetVirtualPath().ToString();
	std::string pathSTDString(TCHAR_TO_UTF8(*path));
	std::size_t dotPos = pathSTDString.find_last_of(".");
	pathSTDString = pathSTDString.substr(0, dotPos) + ".uasset";
	pathSTDString = pathSTDString.substr(5, pathSTDString.length() - 5);
	path = path.Right(path.Len() - 5) + ".uasset";
	path = FString(pathSTDString.c_str());

	//UE_LOG(MyLog, Warning, TEXT("filePath:%s"), *path); //debug

	DBApi::getOneTagByPlantNameRequest request;
	request.plantName = path;
	request.projName = FApp::GetProjectName();
	request.tag = filterKey;
	if (filterType.Equals("Color"))
	{
		request.type = DBApi::tagType::NUMBER;
	}
	else 
	{
		request.type = DBApi::tagType::STRING;
	}

	DBApi::getOneTagByPlantNameResult result;
	result = DBApi::getOneTagByPlantName(request);

	//UE_LOG(MyLog, Warning, TEXT("ErrCode :%d"), result.errCode); //debug

	TArray<DBApi::stringKV> stringTags = result.stringTags;
	TArray<DBApi::numberKV> numberTags = result.numberTags;

	if (filterType.Equals("Ecosystem"))
	{
		DBApi::stringKV* ecosysKV = stringTags.FindByPredicate([](DBApi::stringKV kv) { return kv.key == "Ecosystem"; });
		if (!ecosysKV) {
			// ecosystem tag missing
			return false;
		}
		FString ecosysPop, ecosysVal = ecosysKV->value;
		while (ecosysVal.Split("/", &ecosysPop, &ecosysVal)) {
			if (ecosysPop.Equals(filterValue))
			{
				return true;
			}
		}
		if (ecosysVal.Equals(filterValue))
		{
			return true;
		}
	}
	else if (filterType.Equals("Color"))
	{
		float min = FCString::Atof(*filterMinValue);
		float max = FCString::Atof(*filterMaxValue);
		for (int32 Index = 0; Index != numberTags.Num(); ++Index)
		{
			if (numberTags[Index].key.Equals(filterKey))
			{
				float value = numberTags[Index].value;
				if (value >= min && value <= max)
					return true;
			}
		}
	}

	return false;
}

#undef LOCTEXT_NAMESPACE