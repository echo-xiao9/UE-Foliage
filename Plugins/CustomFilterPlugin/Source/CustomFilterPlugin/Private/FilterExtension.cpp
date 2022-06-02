// Fill out your copyright notice in the Description page of Project Settings.


#include "FilterExtension.h"
#include "CustomFilter.h"

void UFilterExtension::AddFrontEndFilterExtensions(TSharedPtr<class FFrontendFilterCategory> DefaultCategory, TArray< TSharedRef<class FFrontendFilter> >& InOutFilterList) const
{
	std::fstream file;
	file.open("D:\\FilterSetting.ini", std::ios::in);
	std::string name, key, value;
	while (file >> name && file >> key && file >> value)
	{
		FString fname(name.c_str());
		FString fkey(key.c_str());
		FString fvalue(value.c_str());
		InOutFilterList.Add(MakeShareable(new CustomFilter(DefaultCategory, fname, fkey, fvalue)));
	}
	/*InOutFilterList.Add(MakeShareable(new CustomFilter(DefaultCategory, "FoliageColor_Green", "Color", "Green")));
	InOutFilterList.Add(MakeShareable(new CustomFilter(DefaultCategory, "FoliageColor_White", "Color", "White")));
	InOutFilterList.Add(MakeShareable(new CustomFilter(DefaultCategory, "FoliageColor_Yellow", "Color", "Yellow")));

	InOutFilterList.Add(MakeShareable(new CustomFilter(DefaultCategory, "Height_Tall", "Height", "Tall")));
	InOutFilterList.Add(MakeShareable(new CustomFilter(DefaultCategory, "Height_Short", "Height", "Short")));*/
}