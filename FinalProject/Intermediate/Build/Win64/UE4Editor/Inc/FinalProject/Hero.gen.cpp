// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "FinalProject/Hero.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeHero() {}
// Cross Module References
	FINALPROJECT_API UEnum* Z_Construct_UEnum_FinalProject_State();
	UPackage* Z_Construct_UPackage__Script_FinalProject();
	FINALPROJECT_API UEnum* Z_Construct_UEnum_FinalProject_Input();
	FINALPROJECT_API UClass* Z_Construct_UClass_AHero_NoRegister();
	FINALPROJECT_API UClass* Z_Construct_UClass_AHero();
	ENGINE_API UClass* Z_Construct_UClass_APawn();
	ENGINE_API UClass* Z_Construct_UClass_USphereComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UStaticMeshComponent_NoRegister();
// End Cross Module References
	static UEnum* State_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_FinalProject_State, Z_Construct_UPackage__Script_FinalProject(), TEXT("State"));
		}
		return Singleton;
	}
	template<> FINALPROJECT_API UEnum* StaticEnum<State>()
	{
		return State_StaticEnum();
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_State(State_StaticEnum, TEXT("/Script/FinalProject"), TEXT("State"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_FinalProject_State_Hash() { return 3136474393U; }
	UEnum* Z_Construct_UEnum_FinalProject_State()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_FinalProject();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("State"), 0, Get_Z_Construct_UEnum_FinalProject_State_Hash(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "State::Idle", (int64)State::Idle },
				{ "State::Move", (int64)State::Move },
				{ "State::Jump", (int64)State::Jump },
				{ "State::Dodge", (int64)State::Dodge },
				{ "State::Defeat", (int64)State::Defeat },
				{ "State::Max", (int64)State::Max },
			};
#if WITH_METADATA
			const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "Defeat.Name", "State::Defeat" },
				{ "Dodge.Name", "State::Dodge" },
				{ "Idle.Name", "State::Idle" },
				{ "Jump.Name", "State::Jump" },
				{ "Max.Hidden", "" },
				{ "Max.Name", "State::Max" },
				{ "ModuleRelativePath", "Hero.h" },
				{ "Move.Name", "State::Move" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_FinalProject,
				nullptr,
				"State",
				"State",
				Enumerators,
				UE_ARRAY_COUNT(Enumerators),
				RF_Public|RF_Transient|RF_MarkAsNative,
				EEnumFlags::None,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				(uint8)UEnum::ECppForm::EnumClass,
				METADATA_PARAMS(Enum_MetaDataParams, UE_ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	static UEnum* Input_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_FinalProject_Input, Z_Construct_UPackage__Script_FinalProject(), TEXT("Input"));
		}
		return Singleton;
	}
	template<> FINALPROJECT_API UEnum* StaticEnum<Input>()
	{
		return Input_StaticEnum();
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_Input(Input_StaticEnum, TEXT("/Script/FinalProject"), TEXT("Input"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_FinalProject_Input_Hash() { return 2015507676U; }
	UEnum* Z_Construct_UEnum_FinalProject_Input()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_FinalProject();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("Input"), 0, Get_Z_Construct_UEnum_FinalProject_Input_Hash(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "Input::Up", (int64)Input::Up },
				{ "Input::Down", (int64)Input::Down },
				{ "Input::Left", (int64)Input::Left },
				{ "Input::Right", (int64)Input::Right },
				{ "Input::Jump", (int64)Input::Jump },
				{ "Input::Dodge", (int64)Input::Dodge },
				{ "Input::Action", (int64)Input::Action },
				{ "Input::Swap", (int64)Input::Swap },
				{ "Input::Menu", (int64)Input::Menu },
				{ "Input::Max", (int64)Input::Max },
			};
#if WITH_METADATA
			const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "Action.Name", "Input::Action" },
				{ "BlueprintType", "true" },
				{ "Dodge.Name", "Input::Dodge" },
				{ "Down.Name", "Input::Down" },
				{ "Jump.Name", "Input::Jump" },
				{ "Left.Name", "Input::Left" },
				{ "Max.Hidden", "" },
				{ "Max.Name", "Input::Max" },
				{ "Menu.Name", "Input::Menu" },
				{ "ModuleRelativePath", "Hero.h" },
				{ "Right.Name", "Input::Right" },
				{ "Swap.Name", "Input::Swap" },
				{ "Up.Name", "Input::Up" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_FinalProject,
				nullptr,
				"Input",
				"Input",
				Enumerators,
				UE_ARRAY_COUNT(Enumerators),
				RF_Public|RF_Transient|RF_MarkAsNative,
				EEnumFlags::None,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				(uint8)UEnum::ECppForm::EnumClass,
				METADATA_PARAMS(Enum_MetaDataParams, UE_ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	void AHero::StaticRegisterNativesAHero()
	{
	}
	UClass* Z_Construct_UClass_AHero_NoRegister()
	{
		return AHero::StaticClass();
	}
	struct Z_Construct_UClass_AHero_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_sphereComponent_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_sphereComponent;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_meshComponent_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_meshComponent;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_speed_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_speed;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AHero_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_APawn,
		(UObject* (*)())Z_Construct_UPackage__Script_FinalProject,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AHero_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "Hero.h" },
		{ "ModuleRelativePath", "Hero.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AHero_Statics::NewProp_sphereComponent_MetaData[] = {
		{ "Category", "Hero" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Hero.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AHero_Statics::NewProp_sphereComponent = { "sphereComponent", nullptr, (EPropertyFlags)0x0010000000080009, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AHero, sphereComponent), Z_Construct_UClass_USphereComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_AHero_Statics::NewProp_sphereComponent_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AHero_Statics::NewProp_sphereComponent_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AHero_Statics::NewProp_meshComponent_MetaData[] = {
		{ "Category", "Hero" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Hero.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AHero_Statics::NewProp_meshComponent = { "meshComponent", nullptr, (EPropertyFlags)0x0010000000080009, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AHero, meshComponent), Z_Construct_UClass_UStaticMeshComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_AHero_Statics::NewProp_meshComponent_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AHero_Statics::NewProp_meshComponent_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AHero_Statics::NewProp_speed_MetaData[] = {
		{ "Category", "Hero" },
		{ "ModuleRelativePath", "Hero.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AHero_Statics::NewProp_speed = { "speed", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AHero, speed), METADATA_PARAMS(Z_Construct_UClass_AHero_Statics::NewProp_speed_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AHero_Statics::NewProp_speed_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AHero_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AHero_Statics::NewProp_sphereComponent,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AHero_Statics::NewProp_meshComponent,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AHero_Statics::NewProp_speed,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_AHero_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AHero>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AHero_Statics::ClassParams = {
		&AHero::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_AHero_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_AHero_Statics::PropPointers),
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AHero_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AHero_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AHero()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AHero_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AHero, 1282427905);
	template<> FINALPROJECT_API UClass* StaticClass<AHero>()
	{
		return AHero::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AHero(Z_Construct_UClass_AHero, &AHero::StaticClass, TEXT("/Script/FinalProject"), TEXT("AHero"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AHero);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
