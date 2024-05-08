// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef FINALPROJECT_Hero_generated_h
#error "Hero.generated.h already included, missing '#pragma once' in Hero.h"
#endif
#define FINALPROJECT_Hero_generated_h

#define FinalProject_Source_FinalProject_Hero_h_39_SPARSE_DATA
#define FinalProject_Source_FinalProject_Hero_h_39_RPC_WRAPPERS
#define FinalProject_Source_FinalProject_Hero_h_39_RPC_WRAPPERS_NO_PURE_DECLS
#define FinalProject_Source_FinalProject_Hero_h_39_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAHero(); \
	friend struct Z_Construct_UClass_AHero_Statics; \
public: \
	DECLARE_CLASS(AHero, APawn, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/FinalProject"), NO_API) \
	DECLARE_SERIALIZER(AHero)


#define FinalProject_Source_FinalProject_Hero_h_39_INCLASS \
private: \
	static void StaticRegisterNativesAHero(); \
	friend struct Z_Construct_UClass_AHero_Statics; \
public: \
	DECLARE_CLASS(AHero, APawn, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/FinalProject"), NO_API) \
	DECLARE_SERIALIZER(AHero)


#define FinalProject_Source_FinalProject_Hero_h_39_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AHero(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AHero) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AHero); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AHero); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AHero(AHero&&); \
	NO_API AHero(const AHero&); \
public:


#define FinalProject_Source_FinalProject_Hero_h_39_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AHero(AHero&&); \
	NO_API AHero(const AHero&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AHero); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AHero); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AHero)


#define FinalProject_Source_FinalProject_Hero_h_39_PRIVATE_PROPERTY_OFFSET
#define FinalProject_Source_FinalProject_Hero_h_37_PROLOG
#define FinalProject_Source_FinalProject_Hero_h_39_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FinalProject_Source_FinalProject_Hero_h_39_PRIVATE_PROPERTY_OFFSET \
	FinalProject_Source_FinalProject_Hero_h_39_SPARSE_DATA \
	FinalProject_Source_FinalProject_Hero_h_39_RPC_WRAPPERS \
	FinalProject_Source_FinalProject_Hero_h_39_INCLASS \
	FinalProject_Source_FinalProject_Hero_h_39_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FinalProject_Source_FinalProject_Hero_h_39_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FinalProject_Source_FinalProject_Hero_h_39_PRIVATE_PROPERTY_OFFSET \
	FinalProject_Source_FinalProject_Hero_h_39_SPARSE_DATA \
	FinalProject_Source_FinalProject_Hero_h_39_RPC_WRAPPERS_NO_PURE_DECLS \
	FinalProject_Source_FinalProject_Hero_h_39_INCLASS_NO_PURE_DECLS \
	FinalProject_Source_FinalProject_Hero_h_39_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> FINALPROJECT_API UClass* StaticClass<class AHero>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FinalProject_Source_FinalProject_Hero_h


#define FOREACH_ENUM_STATE(op) \
	op(State::Idle) \
	op(State::Move) \
	op(State::Jump) \
	op(State::Dodge) \
	op(State::Defeat) \
	op(State::Max) 

enum class State : uint8;
template<> FINALPROJECT_API UEnum* StaticEnum<State>();

#define FOREACH_ENUM_INPUT(op) \
	op(Input::Up) \
	op(Input::Down) \
	op(Input::Left) \
	op(Input::Right) \
	op(Input::Jump) \
	op(Input::Dodge) \
	op(Input::Action) \
	op(Input::Swap) \
	op(Input::Menu) \
	op(Input::Max) 

enum class Input : uint8;
template<> FINALPROJECT_API UEnum* StaticEnum<Input>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
