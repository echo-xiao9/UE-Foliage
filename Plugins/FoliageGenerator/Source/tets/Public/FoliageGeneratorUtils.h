// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TETS_API FoliageGeneratorUtils
{
public:
	enum class PlantLayer { Tree = 1, Bush = 2, Grass = 3, Flower = 4, Stone = 5 };
  enum class EcoSystem
{
    Subtropical = 1,
    Desert = 2,
    Subfrigid = 3,
    Coniferous = 4,
    Tropical = 5,
    Monsoon = 6
};
	FoliageGeneratorUtils();
	~FoliageGeneratorUtils();
};
