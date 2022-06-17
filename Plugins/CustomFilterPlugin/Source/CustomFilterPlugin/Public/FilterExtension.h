// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ContentBrowserFrontEndFilterExtension.h"
#include "Misc/Paths.h"
#include <fstream>
#include <sstream>
#include "FilterExtension.generated.h"

UCLASS()
/// <summary>
/// 该类负责将CustomFilter对象添加到编辑器当中
/// </summary>
class UFilterExtension : public UContentBrowserFrontEndFilterExtension
{
public:
	GENERATED_BODY()

	// UContentBrowserFrontEndFilterExtension interface
	virtual void AddFrontEndFilterExtensions(TSharedPtr<class FFrontendFilterCategory> DefaultCategory, TArray< TSharedRef<class FFrontendFilter> >& InOutFilterList) const override;
	// End of UContentBrowserFrontEndFilterExtension interface
};

DECLARE_LOG_CATEGORY_EXTERN(MyLog, Log, All);