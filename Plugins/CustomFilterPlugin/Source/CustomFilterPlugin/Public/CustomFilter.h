// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "FrontendFilterBase.h"
#include "DBApi.h"
#include "Misc/App.h"
/**
 *
 */
class CustomFilter : public FFrontendFilter
{
public:
	CustomFilter(TSharedPtr<FFrontendFilterCategory> InCategory, FString name, FString key, FString value) : FFrontendFilter(InCategory)
	{
		filterName = name;
		filterKey = key;
		filterValue = value;
	}

	// FFrontendFilter implementation
	virtual FLinearColor GetColor() const override;
	virtual FString GetName() const override;
	virtual FText GetDisplayName() const override;
	virtual FText GetToolTipText() const override;

	// IFilter implementation
	virtual bool PassesFilter(FAssetFilterType InItem) const override;

private:
	FString filterName;
	FString filterKey;
	FString filterValue;

};

DECLARE_LOG_CATEGORY_EXTERN(MyLog, Log, All);
