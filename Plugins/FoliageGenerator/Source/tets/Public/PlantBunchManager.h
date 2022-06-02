// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlantBunch.h"
#include "Landscape.h"
#include "Math/UnrealMathUtility.h"
#include "PlantBunchManager.generated.h"

UCLASS()
class TETS_API APlantBunchManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlantBunchManager();
	UPROPERTY(EditAnywhere, Category = "Generate...")
		UPlantBunch* plantBunch;
    
    UPROPERTY(EditAnywhere, Category = "MainTree")
    AActor* mainTree;

private:
	float GetLandscapeHeight(FVector spawnLocation, float radius, float maxDeltaHeight, bool& smoothLandscape);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Generate();
    void GenerateSingle(FVector centerPos, float radius, float angle,int type);
};
