// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomFilter.h"

#define LOCTEXT_NAMESPACE "CustomFilter"

DEFINE_LOG_CATEGORY(MyLog);

FLinearColor CustomFilter::GetColor() const
{
	/*if (filterKey.Equals("Color") && filterValue.Equals("Yellow"))
		return FLinearColor::Yellow;
	if (filterKey.Equals("Color") && filterValue.Equals("White"))
		return FLinearColor::White;
	if (filterKey.Equals("Color") && filterValue.Equals("Green"))
		return FLinearColor::Green;*/

	//�������ɫtag������hֵ���ɹ�����tag��ɫ
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
		//��filterName��Ϊ��������һ����ɫֵ��
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
	return LOCTEXT("Filter", "This is a custom filter");
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
	//��Ҫ����ͷ��Gameȥ������β��ԭ��׺�ĵ���Ϊʲô���к��ļ���һ���ĺ�׺?������β���� .uaasset
	FString path = InItem.GetVirtualPath().ToString();
	std::string pathSTDString(TCHAR_TO_UTF8(*path));
	std::size_t dotPos = pathSTDString.find_last_of(".");
	pathSTDString = pathSTDString.substr(0, dotPos) + ".uasset";
	pathSTDString = pathSTDString.substr(5, pathSTDString.length() - 5);
	path = path.Right(path.Len() - 5) + ".uasset";
	path = FString(pathSTDString.c_str());

	//UE_LOG(MyLog, Warning, TEXT("filePath:%s"), *path); //debug

	//DBApi::getAllTagsByPlantNameRequest request;
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

		//for (int32 Index = 0; Index != stringTags.Num(); ++Index)
		//{
		//	if (stringTags[Index].key.Equals(filterKey))
		//	{
		//		FString ecosysPop, ecosysVal = stringTags[Index].value;
		//		while (ecosysVal.Split("/", &ecosysPop, &ecosysVal)) {
		//			if (ecosysPop.Equals(filterValue))
		//			{
		//				return true;
		//			}
		//		}
		//		/*if (stringTags[Index].value.Equals(filterValue))
		//		{
		//			return true;
		//		}*/
		//	}
		//}
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