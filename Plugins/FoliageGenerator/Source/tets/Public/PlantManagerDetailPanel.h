// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"


// This class does not need to be modified.
class IDetailLayoutBuilder;
class TETS_API FPlantManagerDetailPanel: public IDetailCustomization
{

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();
private:
	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
};