// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantBunch.h"

void UPlantBunch::recommendPlant() {
    FString mainBPPath = "";
    FString ecoString="";
    switch (ecoSystem) {
    case EcoSystem::Subtropical: { //Subtropical
        mainBPPath = "/MyGame/Art/NaturePackage/Meshes/flora/BP_Tree06.uasset";
        subTreeNum = 2;
        bushNum = 1;
        grassNum = 8;
        flowerNum = 5;
        stoneNum = 1;
        radius = 800;
        center = 5;
        ecoString = "Subtropical Test";
        break;
    }
    case EcoSystem::Desert: //Desert
    {
        mainBPPath = "/MyGame/Art/RedDesertPack/meshes/Flora/BP_JniperTree01.uasset";
        subTreeNum = 2;
        bushNum = 3;
        grassNum = 5;
        flowerNum = 0;
        stoneNum = 2;
        radius = 600;
        center = 1;
        ecoString = "Desert Test";
        break;
    }
    case EcoSystem::Coniferous: // Subfrigid
    {
      
        mainBPPath = "/MyGame/Art/NaturePackage/Meshes/flora/BP_Conifer1.uasset";
        subTreeNum = 2;
        bushNum = 2;
        grassNum = 6;
        flowerNum = 6;
        stoneNum = 2;
        radius = 700;
        center = 3;
        ecoString = "Coniferous Test";
        break;
    }
    case EcoSystem::Tropical: // Subfrigid
    {
        mainBPPath = "/MyGame/Art/Tropical_Jungle_Pack/Meshes/Trees/BP_SM_PalmCoconut_01.uasset";
        subTreeNum = 2;
        bushNum = 4;
        grassNum = 10;
        flowerNum = 0;
        stoneNum = 2;
        radius = 300;
        center = 3;
        ecoString = "Tropical Test";
        break;
    }

    default:
        break;
    }

        UPlantRecommend* testObjective = NewObject<UPlantRecommend>();

        subTrees = testObjective->GetRecommendPlantsBP(mainBPPath, FoliageGeneratorUtils::PlantLayer::Tree,ecoString);
        bushes = testObjective->GetRecommendPlantsBP(mainBPPath, FoliageGeneratorUtils::PlantLayer::Bush,ecoString);
        grasses = testObjective->GetRecommendPlantsBP(mainBPPath, FoliageGeneratorUtils::PlantLayer::Grass,ecoString);
        flowers = testObjective->GetRecommendPlantsBP(mainBPPath, FoliageGeneratorUtils::PlantLayer::Flower,ecoString);
        stones = testObjective->GetRecommendPlantsBP(mainBPPath, FoliageGeneratorUtils::PlantLayer::Stone,ecoString);



        UE_LOG(LogClass, Log, TEXT("recommend"));

    };


    