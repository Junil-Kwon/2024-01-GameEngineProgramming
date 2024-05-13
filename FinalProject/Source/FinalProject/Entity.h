#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Entity.generated.h"



UENUM(BlueprintType)
enum class Identifier : uint8 {
	None			= 0,
	Hero			,
	Indicator		,
	Dust			,
	Max				UMETA(Hidden),
};

UENUM(BlueprintType)
enum class Sprite : uint8 {
	None			= 0,
	Idle			,
	Move			,
	Jump			,
	Dodge			,
	Defeat			,
	Max				UMETA(Hidden),
};

UENUM(BlueprintType)
enum class State : uint8 {
	Healthy			= 0,
	Dead			,
	Max				UMETA(Hidden),
};

UENUM(BlueprintType)
enum class Group : uint8 {
	None			= 0,
	Friendly		,
	Enemy			,
	Max				UMETA(Hidden),
};

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class Tag : uint8 {
	None			= 0 UMETA(Hidden),
	Floating		= 1 << 0,
	Piercing		= 1 << 1,
	Invulnerability	= 1 << 2,
	Interactability	= 1 << 3,
	Player			= 1 << 4,
	Leader			= 1 << 5,
	Max				= 6 UMETA(Hidden),
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
	Slowness		= 1 << 7,
	Max				= 8 UMETA(Hidden),
};
ENUM_CLASS_FLAGS(Effect);



template <typename T> uint8 ToInt (T     value);
template <typename T> T     ToEnum(uint8 value);
FRotator VectorToRotator(FVector  value);
FVector  RotatorToVector(FRotator value);



UCLASS(HideCategories = ("Actor Tick", Replication, Rendering, Collision, Actor, Input, LOD, Cooking))
class FINALPROJECT_API AEntity : public AActor {
	GENERATED_BODY()

public:
	bool operator==(const AEntity& other) const;
	UStaticMesh* GetPlaneMesh ();
	UStaticMesh* GetSphereMesh();
	UMaterialInstance* GetMaterialInstance();
	AEntity* Spawn(Identifier value, FVector location = FVector::ZeroVector);



public:
	AEntity();
protected:
	virtual void BeginPlay() override;

	// Physics
	#define ParticleThreshold 500.0f
private:
	float yPrev = 0.0f;
	float fallSpeed = 0.0f;
	bool  isFalling = false;
public:
	virtual void Tick(float DeltaTime) override;
	virtual bool IsFalling();



	// Identifier
private:
	UPROPERTY(EditAnywhere) Identifier identifier;

	// Hitbox
	#define DefaultHitboxRadius 0.5f
	#define DefaultHitboxHeight 1.0f
private:
	UPROPERTY() class UCapsuleComponent* hitboxComponent;
	UPROPERTY(EditAnywhere, Category = "Hitbox") float hitboxRadius = DefaultHitboxRadius;
	UPROPERTY(EditAnywhere, Category = "Hitbox") float hitboxHeight = DefaultHitboxHeight;
	UPROPERTY(EditAnywhere, Category = "Hitbox") float mass = 1.0f;
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

	// Sprite
private:
	UPROPERTY() class UStaticMeshComponent* spriteComponent;
	UPROPERTY(EditAnywhere, Category = "Sprite") class UMaterialInstance* materialInstance;
	int32 spriteCount = 0;
	int32 spriteIndex = 0;
	bool  spriteXFlip = false;
protected:
	float spriteDelay = 0.0f;
	int32 GetSpriteCount();
	int32 GetSpriteIndex();
	bool  GetSpriteXFlip();
	void  SetSpriteIndex(int32 value);
	void  SetSpriteXFlip(bool  value);
	virtual void UpdateSprite(float DeltaTime);

	// Shadow
private:
	UPROPERTY() class UStaticMeshComponent* shadowComponent;



	// State
private:
	UPROPERTY(EditAnywhere) State state = State::Healthy;
public:
	State GetState();
	virtual void SetState(State value);

	// Group
private:
	UPROPERTY(EditAnywhere) Group group = Group::None;
public:
	Group GetGroup();
	virtual void SetGroup(Group value);

	// Tag
private:
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Tag)) uint8 tag;
public:
	bool HasTag(Tag value);
	virtual bool AddTag(Tag value);
	virtual bool RemoveTag(Tag value);

	// Effect
	#define EffectMaxDuration 9999.0f
private:
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Effect)) uint8 effect         = 0;
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Effect)) uint8 effectImmunity = 0;
	TArray<float> effectStrength;
	TArray<float> effectDuration;
protected:
	uint8 GetIndex(Effect value);
	float GetEffectStrength(uint8 value);
	float GetEffectDuration(uint8 value);
	void  SetEffectStrength(uint8 value, float strength);
	void  SetEffectDuration(uint8 value, float duration = EffectMaxDuration);
	virtual void UpdateEffect(float DeltaTime);
public:
	bool HasEffect(Effect value);
	virtual bool AddEffect(Effect value, float strength, float duration = EffectMaxDuration);
	virtual bool RemoveEffect(Effect value);

	// Movement
	#define DefaultSpeed 400.0f
protected:
	UPROPERTY(EditAnywhere) float speed = DefaultSpeed;
	FVector direction = FVector::ZeroVector;
	//float GetSpeed();
};
