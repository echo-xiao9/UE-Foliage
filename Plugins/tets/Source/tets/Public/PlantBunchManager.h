// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlantBunch.h"
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
		

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Generate();
};
