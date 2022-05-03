// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "tets/Public/PlantBunchManager.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePlantBunchManager() {}
// Cross Module References
	TETS_API UClass* Z_Construct_UClass_APlantBunchManager_NoRegister();
	TETS_API UClass* Z_Construct_UClass_APlantBunchManager();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_tets();
	TETS_API UClass* Z_Construct_UClass_UPlantBunch_NoRegister();
// End Cross Module References
	void APlantBunchManager::StaticRegisterNativesAPlantBunchManager()
	{
	}
	UClass* Z_Construct_UClass_APlantBunchManager_NoRegister()
	{
		return APlantBunchManager::StaticClass();
	}
	struct Z_Construct_UClass_APlantBunchManager_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_plantBunch_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_plantBunch;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_APlantBunchManager_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_tets,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_APlantBunchManager_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "PlantBunchManager.h" },
		{ "ModuleRelativePath", "Public/PlantBunchManager.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_APlantBunchManager_Statics::NewProp_plantBunch_MetaData[] = {
		{ "Category", "Generate..." },
		{ "ModuleRelativePath", "Public/PlantBunchManager.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_APlantBunchManager_Statics::NewProp_plantBunch = { "plantBunch", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(APlantBunchManager, plantBunch), Z_Construct_UClass_UPlantBunch_NoRegister, METADATA_PARAMS(Z_Construct_UClass_APlantBunchManager_Statics::NewProp_plantBunch_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_APlantBunchManager_Statics::NewProp_plantBunch_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_APlantBunchManager_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_APlantBunchManager_Statics::NewProp_plantBunch,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_APlantBunchManager_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<APlantBunchManager>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_APlantBunchManager_Statics::ClassParams = {
		&APlantBunchManager::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_APlantBunchManager_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_APlantBunchManager_Statics::PropPointers),
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_APlantBunchManager_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_APlantBunchManager_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_APlantBunchManager()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_APlantBunchManager_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(APlantBunchManager, 2448837478);
	template<> TETS_API UClass* StaticClass<APlantBunchManager>()
	{
		return APlantBunchManager::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_APlantBunchManager(Z_Construct_UClass_APlantBunchManager, &APlantBunchManager::StaticClass, TEXT("/Script/tets"), TEXT("APlantBunchManager"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(APlantBunchManager);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
