// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "tets/Public/PlantBunch.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePlantBunch() {}
// Cross Module References
	TETS_API UClass* Z_Construct_UClass_UPlantBunch_NoRegister();
	TETS_API UClass* Z_Construct_UClass_UPlantBunch();
	ENGINE_API UClass* Z_Construct_UClass_UDataAsset();
	UPackage* Z_Construct_UPackage__Script_tets();
	ENGINE_API UClass* Z_Construct_UClass_UBlueprint_NoRegister();
// End Cross Module References
	void UPlantBunch::StaticRegisterNativesUPlantBunch()
	{
	}
	UClass* Z_Construct_UClass_UPlantBunch_NoRegister()
	{
		return UPlantBunch::StaticClass();
	}
	struct Z_Construct_UClass_UPlantBunch_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Metadata_MetaData[];
#endif
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_Metadata;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_blueprint_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_blueprint;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UPlantBunch_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UDataAsset,
		(UObject* (*)())Z_Construct_UPackage__Script_tets,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPlantBunch_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "PlantBunch.h" },
		{ "ModuleRelativePath", "Public/PlantBunch.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPlantBunch_Statics::NewProp_Metadata_MetaData[] = {
		{ "Category", "Metadata" },
		{ "ModuleRelativePath", "Public/PlantBunch.h" },
	};
#endif
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UClass_UPlantBunch_Statics::NewProp_Metadata = { "Metadata", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPlantBunch, Metadata), METADATA_PARAMS(Z_Construct_UClass_UPlantBunch_Statics::NewProp_Metadata_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPlantBunch_Statics::NewProp_Metadata_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPlantBunch_Statics::NewProp_blueprint_MetaData[] = {
		{ "Category", "Appearence" },
		{ "ModuleRelativePath", "Public/PlantBunch.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UPlantBunch_Statics::NewProp_blueprint = { "blueprint", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPlantBunch, blueprint), Z_Construct_UClass_UBlueprint_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UPlantBunch_Statics::NewProp_blueprint_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPlantBunch_Statics::NewProp_blueprint_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UPlantBunch_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPlantBunch_Statics::NewProp_Metadata,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPlantBunch_Statics::NewProp_blueprint,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UPlantBunch_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UPlantBunch>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UPlantBunch_Statics::ClassParams = {
		&UPlantBunch::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UPlantBunch_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UPlantBunch_Statics::PropPointers),
		0,
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UPlantBunch_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UPlantBunch_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UPlantBunch()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UPlantBunch_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UPlantBunch, 3408830008);
	template<> TETS_API UClass* StaticClass<UPlantBunch>()
	{
		return UPlantBunch::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPlantBunch(Z_Construct_UClass_UPlantBunch, &UPlantBunch::StaticClass, TEXT("/Script/tets"), TEXT("UPlantBunch"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPlantBunch);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
