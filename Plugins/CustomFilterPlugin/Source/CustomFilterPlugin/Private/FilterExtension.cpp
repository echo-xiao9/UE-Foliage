// Fill out your copyright notice in the Description page of Project Settings.


#include "FilterExtension.h"
#include "CustomFilter.h"

void UFilterExtension::AddFrontEndFilterExtensions(TSharedPtr<class FFrontendFilterCategory> DefaultCategory, TArray< TSharedRef<class FFrontendFilter> >& InOutFilterList) const
{
	std::fstream file;

	FString projectPath = FPaths::GetProjectFilePath();
	std::string pathSTDString(TCHAR_TO_UTF8(*projectPath));
	std::size_t filenamePos = pathSTDString.find_last_of("/");//FilterSetting.ini
	pathSTDString = pathSTDString.substr(0, filenamePos) + "/FilterSetting.ini";

	//UE_LOG(MyLog, Warning, TEXT("projectPath:%s"), *pathSTDString); //debug

	file.open(pathSTDString, std::ios::in);
	std::string line;
	std::string name, type, key, value;
	while (std::getline(file, line))
	{
		//Ö§³ÖË«Ð±¸Ü×¢ÊÍ
		if (line.length() >= 2 && line[0] == '/' && line[1] == '/')
			continue;
		std::stringstream sstream(line);
		if (sstream >> name && sstream >> type) 
		{
			if(type == "Color")
			{
				std::string minValue,maxValue;
				if (sstream >> key && sstream >> minValue && sstream >> maxValue)
				{
					FString fname(name.c_str());
					FString ftype(type.c_str());
					FString fkey(key.c_str());
					FString fminValue(minValue.c_str());
					FString fmaxValue(maxValue.c_str());
					InOutFilterList.Add(MakeShareable(new CustomFilter(DefaultCategory, fname, ftype, fkey, fminValue, fmaxValue)));
				}
			}
			else
			{
				if (sstream >> key && sstream >> value)
				{
					FString fname(name.c_str());
					FString ftype(type.c_str());
					FString fkey(key.c_str());
					FString fvalue(value.c_str());
					InOutFilterList.Add(MakeShareable(new CustomFilter(DefaultCategory, fname, ftype, fkey, fvalue)));
				}
			}
			
		}
		line.clear();
	}
	/*while (file >> name && file >> key && file >> value)
	{
		FString fname(name.c_str());
		FString fkey(key.c_str());
		FString fvalue(value.c_str());
		InOutFilterList.Add(MakeShareable(new CustomFilter(DefaultCategory, fname, fkey, fvalue)));
	}*/
	/*InOutFilterList.Add(MakeShareable(new CustomFilter(DefaultCategory, "FoliageColor_Green", "Color", "Green")));
	InOutFilterList.Add(MakeShareable(new CustomFilter(DefaultCategory, "FoliageColor_White", "Color", "White")));
	InOutFilterList.Add(MakeShareable(new CustomFilter(DefaultCategory, "FoliageColor_Yellow", "Color", "Yellow")));

	InOutFilterList.Add(MakeShareable(new CustomFilter(DefaultCategory, "Height_Tall", "Height", "Tall")));
	InOutFilterList.Add(MakeShareable(new CustomFilter(DefaultCategory, "Height_Short", "Height", "Short")));*/
}