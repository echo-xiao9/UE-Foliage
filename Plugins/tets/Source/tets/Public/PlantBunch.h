// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlantBunch.generated.h"
/**
 * 
 */

UCLASS()
class TETS_API UPlantBunch: public UDataAsset
{
	
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Metadata")
		int Metadata;

	UPROPERTY(EditAnywhere, Category = "MainTree")
		UBlueprint* mainTree;
  UPROPERTY(EditAnywhere, Category = "SubTree")
    UBlueprint* subTree0;
  UPROPERTY(EditAnywhere, Category = "SubTree")
		int number0;
  UPROPERTY(EditAnywhere, Category = "SubTree")
    UBlueprint* subTree1;
  UPROPERTY(EditAnywhere, Category = "SubTree")
		int number1;
  UPROPERTY(EditAnywhere, Category = "SubTree")
    UBlueprint* subTree2;
  UPROPERTY(EditAnywhere, Category = "SubTree")
		int number2;

  UPROPERTY(EditAnywhere, Category = "Radius")
    float radius;

};
