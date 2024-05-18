#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	Pinned			= 1 << 2,
	Invulnerability	= 1 << 3,
	Interactability	= 1 << 4,
	Collectable		= 1 << 5,
	Player			= 1 << 6,
	Leader			= 1 << 7,
	Length			= 8 UMETA(Hidden),
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



UCLASS(HideCategories = ("Actor Tick", Replication, Rendering, Collision, Actor, Input, LOD, Cooking))
class FINALPROJECT_API AEntity : public AActor {
	GENERATED_BODY()

public:
	bool operator==(const AEntity& other) const;
	template<typename TEnum> FString ToFString(TEnum   value);
	template<typename TEnum> TEnum   ToEnum   (FString value);
	FRotator ToRotator(FVector  value);
	FVector  ToVector (FRotator value);

	UStaticMesh* GetPlaneMesh ();
	UStaticMesh* GetSphereMesh();
	UTexture* GetTexture(Identifier value);
	UClass* GetBlueprint(Identifier value);
	UMaterialInstance* GetMaterialInstance();

	UFUNCTION() void CreateMaterial(UStaticMeshComponent* comp = nullptr);
	UFUNCTION() void SetSpriteIndex(UStaticMeshComponent* comp = nullptr, int32   value = 0);
	UFUNCTION() void SetSpriteXFlip(UStaticMeshComponent* comp = nullptr, bool    value = false);
	UFUNCTION() void SetSpriteColor(UStaticMeshComponent* comp = nullptr, FVector value = FVector::OneVector);
	UFUNCTION() void SetSpriteLight(UStaticMeshComponent* comp = nullptr, float   value = 0.0f);

	AEntity* SpawnEntity(Identifier value, FVector location = FVector::ZeroVector);



public:
	AEntity();
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;



	// Identifier
private:
	Identifier identifier = Identifier::Default;
public:
	Identifier GetIdentifier();

	// Hitbox
protected:
	UPROPERTY(EditAnywhere, Category = "Hitbox") float defaultHitboxRadius =  50.0f;
	UPROPERTY(EditAnywhere, Category = "Hitbox") float defaultHitboxHeight = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Hitbox") float defaultMass = 1.0f;
private:
	UPROPERTY() class UCapsuleComponent* hitboxComponent;
	float hitboxRadius;
	float hitboxHeight;
	float mass;
protected:
	virtual void OnHitboxChanged();
public:
	float GetHitboxRadius();
	float GetHitboxHeight();
	void  SetHitboxRadius(float value);
	void  SetHitboxHeight(float value);
	void  SetHitbox(float radius, float height);
	float GetMass();
	void  SetMass(float value);
	void  SetCollisionProfileName(FName value);
	FVector GetFootLocation();

protected:
	TArray<AEntity*> hitArray;
public:
	virtual void OnInteract(AEntity* other);
	UFUNCTION() virtual void OnHit(
		UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);

	// Sprite
private:
	UPROPERTY() class UStaticMeshComponent* spriteComponent;
protected:
	int32   spriteIndex = 0;
	bool    spriteXFlip = false;
	FVector spriteColor = FVector::OneVector;
	float   spriteLight = 0.0f;

	// Shadow
private:
	UPROPERTY() class UStaticMeshComponent* shadowComponent;



	// Physics
	#define Gravity            -980.0f
	#define ParticleThreshold  -500.0f
	#define VoidThreshold     -1000.0f
private:
	UPROPERTY() class AGhost* ghost;
	float zPrevious = 0.0f;
	float fallSpeed = 0.0f;
	bool  isFalling = false;
public:
	UFUNCTION() AGhost* GetGhost();
	float GetWorldSpeed();
	void  SetWorldSpeed(float value);
	virtual void Tick(float DeltaTime) override;
	bool IsFalling();

	// Movement
	#define DefaultSpeed        300.0f
protected:
	UPROPERTY(EditAnywhere) float defaultSpeed = DefaultSpeed;
private:
	float speed;
	FVector direction = FVector::ZeroVector;
	FVector velocity  = FVector::ZeroVector;
	FVector force     = FVector::ZeroVector;
	FVector locationPrevious = FVector::ZeroVector;
public:
	FVector GetDirection();
	void    SetDirection(FVector value);
	void    AddForce    (FVector value);



	// Input
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
	uint8        GetIndex (Tag value);
	bool         HasTag   (Tag value);
	virtual bool AddTag   (Tag value);
	virtual bool RemoveTag(Tag value);

	// Effect
	#define MaxStrength 9999.0f
	#define MaxDuration 9999.0f
protected:
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Effect)) uint8 defaultEffect = 0;
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Effect)) uint8 defaultEffectImmunity = 0;
private:
	uint8 effect;
	uint8 effectImmunity;
	float effectStrength[static_cast<uint8>(Effect::Length)] = { 0, };
	float effectDuration[static_cast<uint8>(Effect::Length)] = { 0, };
	FVector pinned = FVector::ZeroVector;
protected:
	virtual bool UpdateEffect(float DeltaTime);
public:
	uint8        GetIndex    (Effect value);
	bool         HasEffect   (Effect value);
	virtual bool AddEffect   (Effect value, float strength = MaxStrength, float duration = MaxDuration);
	virtual bool RemoveEffect(Effect value);

	float GetEffectStrength   (uint8 value);
	float GetEffectDuration   (uint8 value);
	void  AdjustEffectStrength(uint8 value, float strength);
	void  AdjustEffectDuration(uint8 value, float duration);
};
