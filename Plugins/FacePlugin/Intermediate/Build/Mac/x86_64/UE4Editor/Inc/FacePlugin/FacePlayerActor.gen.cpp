// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "FacePlugin/Public/FacePlayerActor.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeFacePlayerActor() {}
// Cross Module References
	FACEPLUGIN_API UClass* Z_Construct_UClass_AFacePlayerActor_NoRegister();
	FACEPLUGIN_API UClass* Z_Construct_UClass_AFacePlayerActor();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_FacePlugin();
// End Cross Module References
	void AFacePlayerActor::StaticRegisterNativesAFacePlayerActor()
	{
	}
	UClass* Z_Construct_UClass_AFacePlayerActor_NoRegister()
	{
		return AFacePlayerActor::StaticClass();
	}
	struct Z_Construct_UClass_AFacePlayerActor_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AFacePlayerActor_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_FacePlugin,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AFacePlayerActor_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "FacePlayerActor.h" },
		{ "ModuleRelativePath", "Public/FacePlayerActor.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AFacePlayerActor_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AFacePlayerActor>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AFacePlayerActor_Statics::ClassParams = {
		&AFacePlayerActor::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AFacePlayerActor_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AFacePlayerActor_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AFacePlayerActor()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AFacePlayerActor_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AFacePlayerActor, 4117032564);
	template<> FACEPLUGIN_API UClass* StaticClass<AFacePlayerActor>()
	{
		return AFacePlayerActor::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AFacePlayerActor(Z_Construct_UClass_AFacePlayerActor, &AFacePlayerActor::StaticClass, TEXT("/Script/FacePlugin"), TEXT("AFacePlayerActor"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AFacePlayerActor);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
