#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Entity.generated.h"





UENUM(BlueprintType)
enum class Identifier : uint8 {
	Default			= 0,
	Hero			,
	Object			,
	Indicator		,
	Dust			,
	Length			UMETA(Hidden),
};

UENUM(BlueprintType)
enum class Action : uint8 {
	Idle			= 0,
	Move			,
	Jump			,
	Dash			,
	Attack			,
	Defend			,
	Defeat			,
	Length			UMETA(Hidden),
};

UENUM(BlueprintType)
enum class Group : uint8 {
	None			= 0,
	Friendly		,
	Enemy			,
	Length			UMETA(Hidden),
};

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class Tag : uint8 {
	None			= 0 UMETA(Hidden),
	Floating		= 1 << 0,
	Piercing        = 1 << 1,
	Invulnerability	= 1 << 2,
	Interactability	= 1 << 3,
	Collectable		= 1 << 4,
	Player			= 1 << 5,
	Leader			= 1 << 6,
	Length			= 7 UMETA(Hidden),
};
ENUM_CLASS_FLAGS(Tag);

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class Effect : uint8 {
	None			= 0 UMETA(Hidden),
	HealthBoost		= 1 << 0,
	DamageBoost		= 1 << 1,
	Resistance		= 1 << 2,
	Speed			= 1 << 3,
	Burn			= 1 << 4,
	Stun			= 1 << 5,
	Freeze			= 1 << 6,
	Length			= 7 UMETA(Hidden),
};
ENUM_CLASS_FLAGS(Effect);





UCLASS(HideCategories = ("Actor Tick", Character, Camera, Pawn, Replication, Rendering, Collision,
	Actor, Input, LOD, Cooking))
class FINALPROJECT_API AEntity : public ACharacter {
	GENERATED_BODY()

	// =========================================================================================================
	// Entity Functions
	// =========================================================================================================
	// -
public:
	bool operator==(const AEntity& other) const;
	FRotator ToRotator(FVector  value);
	FVector  ToVector (FRotator value);

	uint8 GetIndex(Tag    value);
	uint8 GetIndex(Effect value);
	template<typename TEnum> FString ToFString(TEnum   value);
	template<typename TEnum> TEnum   ToEnum   (FString value);

	UStaticMesh* GetPlaneMesh ();
	UStaticMesh* GetSphereMesh();
	UTexture* GetTexture(Identifier value);
	UClass* GetBlueprint(Identifier value);
	UMaterialInstance* GetMaterialInstance();

	AEntity* Spawn(Identifier value, FVector location = FVector::ZeroVector);





	// =========================================================================================================
	// Default Functions
	// =========================================================================================================
	// Initialization
public:
	AEntity();
protected:
	virtual void BeginPlay() override;

	// Update
	#define MaxFallSpeed  -1024.0f
	#define VoidThreshold  -500.0f
	#define DustThreshold  -500.0f
protected:
	UPROPERTY(EditAnywhere) float defaultSpeed = 300.0f;
private:
	float speed;
protected:
	FVector direction;

	bool  isFalling = false;
	float fallSpeed = 0.0f;
public:	
	virtual void Tick(float DeltaTime) override;





	// =========================================================================================================
	// Components
	// =========================================================================================================
	// Hitbox
protected:
	UPROPERTY(EditAnywhere) float defaultHitboxRadius = 0.0f;
	UPROPERTY(EditAnywhere) float defaultHitboxHeight = 0.0f;
private:
	UPROPERTY() class UCapsuleComponent* hitboxComponent;
	float hitboxRadius;
	float hitboxHeight;
protected:
	virtual void OnHitboxChanged();
public:
	float GetHitboxRadius();
	float GetHitboxHeight();
	void  SetHitboxRadius(float value);
	void  SetHitboxHeight(float value);
	void  SetHitbox(float radius, float height);
	void  SetCollisionProfileName(FName value);
	FVector GetFootLocation();
	
	// Arrow
private:
	UPROPERTY() class UArrowComponent* uArrowComponent;
protected:
	FVector GetArrowDirection();
	void    SetArrowDirection(FVector value);

	// Sprite
private:
	UPROPERTY() class UStaticMeshComponent* spriteComponent;
	int32   spriteIndex = 0;
	bool    spriteXFlip = false;
	FVector spriteColor = FVector::OneVector;
	float   spriteIntensity = 0.0f;
protected:
	UFUNCTION() void CreateMaterial(UStaticMeshComponent* comp = nullptr);
	int32   GetSpriteIndex();
	bool    GetSpriteXFlip();
	FVector GetSpriteColor();
	float   GetSpriteIntensity();
	UFUNCTION() void SetSpriteIndex(UStaticMeshComponent* comp = nullptr, int32   value = 0);
	UFUNCTION() void SetSpriteXFlip(UStaticMeshComponent* comp = nullptr, bool    value = false);
	UFUNCTION() void SetSpriteColor(UStaticMeshComponent* comp = nullptr, FVector value = FVector::OneVector);
	UFUNCTION() void SetSpriteIntensity(UStaticMeshComponent* comp = nullptr, float   value = 0.0f);

	// Shadow
private:
	UPROPERTY() class UStaticMeshComponent* shadowComponent;





	// =========================================================================================================
	// AI
	// =========================================================================================================
	// Input
private:
	UPROPERTY() class AGhost* ghost;
public:
	UFUNCTION() AGhost* GetGhost();
protected:
	bool GetInput(Action value);
	FVector GetInputDirection();

	// Action
private:
	Action action = Action::Idle;
protected:
	float  actionDelay = 0.0f;
	Action GetAction();
	bool   SetAction(Action value);
	virtual bool VerifyAction(Action value);
	virtual bool UpdateInputs(float DeltaTime);
	virtual bool UpdateAction(float DeltaTime);





	// =========================================================================================================
	// Properties
	// =========================================================================================================
	// Identifier
private:
	Identifier identifier = Identifier::Default;
public:
	Identifier GetIdentifier();

	// Group
protected:
	UPROPERTY(EditAnywhere) Group defaultGroup = Group::None;
private:
	Group group;
public:
	Group        GetGroup();
	virtual void SetGroup(Group value);

	// Tag
protected:
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Tag)) uint8 defaultTag = 0;
private:
	uint8 tag;
public:
	bool  HasTag(Tag value);
	virtual bool AddTag   (Tag value);
	virtual bool RemoveTag(Tag value);

	// Effect
	#define EffectMaxStrength 9999.0f
	#define EffectMaxDuration 9999.0f
protected:
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Effect)) uint8 defaultEffect         = 0;
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Effect)) uint8 defaultEffectImmunity = 0;
private:
	uint8 effect;
	uint8 effectImmunity;
	float effectStrength[static_cast<uint8>(Effect::Length)] = { 0, };
	float effectDuration[static_cast<uint8>(Effect::Length)] = { 0, };
protected:
	virtual bool UpdateEffect(float DeltaTime);
public:
	bool  HasEffect(Effect value);
	virtual bool AddEffect   (Effect value, float strength = 1.0f, float duration = EffectMaxDuration);
	virtual bool RemoveEffect(Effect value);

	float GetEffectStrength(Effect value);
	float GetEffectDuration(Effect value);
	void  AdjustEffectStrength(Effect value, float strength);
	void  AdjustEffectDuration(Effect value, float duration);
};
