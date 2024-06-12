#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Entity.generated.h"





UENUM(BlueprintType)
enum class MeshType : uint8 {
	Plane			= 0,
	Sphere			,
	Length			UMETA(Hidden),
};

UENUM(BlueprintType)
enum class FontType : uint8 {
	Galmuri7		= 0,
	Galmuri9		,
	Galmuri11		,
	Galmuri11Bold	,
	Length			UMETA(Hidden),
};





UENUM(BlueprintType)
enum class Identifier : uint8 {
	Default			= 0,

	// Heros
	Adventurer		,
	Wizard			,
	Knight			,
	Priest			,
	Necromancer		,
	// Summoned
	StalKnight		,

	// Monsters
	Slime			,
	Skeleton		,

	// Items
	Chest			,
	Money			,
	HealthPotion	,
	EnergyPotion	,
	Armour			, // Unused

	// Weapons
	Sword			,
	Wand			,
	XBow			,
	Spear			,
	GreatSword		,
	Tomahawk		,
	FireWand		,
	FreezeWand		,

	// Projectiles
	Arrow			,
	TomahawkThrown	,
	Debuff			, // Unused
	Fireball		,
	Icicle			,

	// UI
	Interactor		,
	Indicator		,
	Portal			,

	// Particles
	Dust			,
	Flame			,
	Twinkle			,
	HealthUp		,
	EnergyUp		,
	Lightning		,
	DebuffDust		, // Unused
	FireballDust	,
	IcicleDust		,
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
	Revive			,
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
	PlayerParty		= 1 << 5,
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
	Slowness		= 1 << 4,
	Burn			= 1 << 5,
	Stun			= 1 << 6,
	Freeze			= 1 << 7,
	Length			= 8 UMETA(Hidden),
};
ENUM_CLASS_FLAGS(Effect);





UCLASS(HideCategories = ("Actor Tick", Character, Camera, Pawn, Replication, Rendering, Collision,
	Actor, Input, LOD, Cooking))
class DUNGEON_API AEntity : public ACharacter {
	GENERATED_BODY()

	// =========================================================================================================
	// Default
	// =========================================================================================================
	
	// Default
public:
	bool operator==(const AEntity& other) const;

	FVector  RotateVector(FVector  value);
	FVector  ToVector    (FColor   value);
	float    ToAngle     (FVector  value);
	float    GetDistance (AEntity* entity, AEntity* target);

	template<typename TEnum> uint8   GetIndex(TEnum   value);
	template<typename TEnum> FString ToString(TEnum   value);
	template<typename TEnum> TEnum   ToEnum  (FString value);

	// --

	UStaticMesh*              GetStaticMesh             (MeshType   value);
	UTexture2D*               GetTexture2D              (Identifier value);
	UMaterialInstanceDynamic* GetMaterialInstanceDynamic(Identifier value);

	UFont*     GetFont        (FontType value);
	UMaterial* GetFontMaterial(FontType value);

	// Ghost
private:
	class AGhost* ghost;
public:
	AGhost* GetGhost();

	// --

	AEntity* Spawn(Identifier value, FVector location = FVector::ZeroVector);
	void     Despawn();

	bool IsAttached();
	void Attach(AEntity* entity);
	void Detach();





	// =========================================================================================================
	// Setup
	// =========================================================================================================
	
	// Initialization
public:
	AEntity();

	// Spawn
private:
	bool start;
	bool spawn;
protected:
	virtual void OnStart  ();
	virtual void OnSpawn  ();
	virtual void OnDespawn();
public:
	bool Spawned();

	// Update
	#define DefaultGravityScale     3.0f
	#define FallSpeedMax        -1024.0f
	#define VoidZAxis           -1024.0f
	#define DustThreshold        -500.0f
protected:
	UPROPERTY(EditAnywhere) float defaultSpeed;
private:
	float speed;
	bool  isFalling;
	float fallSpeed;
public:
	virtual void Tick(float DeltaTime) override;
	bool IsFalling();
protected:
	virtual void Update(float DeltaTime);

private:
	float lifeTime;
public:
	float GetLifeTime();
protected:
	void  SetLifeTime(float value);

private:
	FVector lookDirection;
	FVector moveDirection;
public:
	FVector GetLookDirection();
	FVector GetMoveDirection();
protected:
	void SetLookDirection(FVector value);
	void SetMoveDirection(FVector value);





	// =========================================================================================================
	// Components
	// =========================================================================================================
	
	// Hitbox
protected:
	UPROPERTY(EditAnywhere) float     defaultHitboxRadius;
	UPROPERTY(EditAnywhere) float     defaultHitboxHeight;
	UPROPERTY(EditAnywhere) FVector2D defaultHandLocation;
private:
	UCapsuleComponent* hitboxComponent;
	float   hitboxRadius;
	float   hitboxHeight;
	FVector handLocation;
protected:
	virtual void OnHitboxChanged();
public:
	float GetHitboxRadius();
	float GetHitboxHeight();
	void  SetHitboxRadius(float value);
	void  SetHitboxHeight(float value);
	void  SetHitbox(float radius, float height);
	void  SetCollisionProfileName(FName value);
	virtual FVector GetHandLocation();
	virtual FVector GetFootLocation();

	UFUNCTION() void OnHit(
		UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);
	UFUNCTION() void OnHitboxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnCollision(AEntity* entity);
	virtual void OnInteract (AEntity* entity);

	// Sprite
private:
	class USceneComponent*      anchorComponent;
	class UStaticMeshComponent* spriteComponent;
	int32   spriteIndex;
	bool    spriteXFlip;
	FVector spriteColor;
	float   spriteIntensity;
	float   spriteOpacity;
	float   spriteAngle;
protected:
	class USceneComponent* GetAnchorComponent();
public:
	int32   GetSpriteIndex();
	bool    GetSpriteXFlip();
	FVector GetSpriteColor();
	float   GetSpriteIntensity();
	float   GetSpriteOpacity();
	float   GetSpriteAngle();
	UFUNCTION() void SetSpriteIndex    (UStaticMeshComponent* comp, int32   value = 0);
	UFUNCTION() void SetSpriteXFlip    (UStaticMeshComponent* comp, bool    value = false);
	UFUNCTION() void SetSpriteColor    (UStaticMeshComponent* comp, FVector value = FVector::OneVector);
	UFUNCTION() void SetSpriteIntensity(UStaticMeshComponent* comp, float   value = 0.25f);
	UFUNCTION() void SetSpriteOpacity  (UStaticMeshComponent* comp, float   value = 1.0f);
	UFUNCTION() void SetSpriteAngle    (UStaticMeshComponent* comp, float   value = 0.0f);

	// Shadow
private:
	class UStaticMeshComponent* shadowComponent;





	// =========================================================================================================
	// Properties
	// =========================================================================================================
	
	// Identifier
private:
	Identifier identifier;
public:
	Identifier GetIdentifier();

	// Log
protected:
	UPROPERTY(EditAnywhere) FString defaultLog;
private:
	FString log;
public:
	FString GetLog();

	// Group
protected:
	UPROPERTY(EditAnywhere) Group defaultGroup;
private:
	Group group;
public:
	Group        GetGroup();
	virtual void SetGroup(Group value);

	// Tag
protected:
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Tag)) uint8 defaultTag;
private:
	uint8 tag;
public:
	bool         HasTag   (Tag value);
	virtual bool AddTag   (Tag value);
	virtual bool RemoveTag(Tag value);

	// Effect
	#define EffectHitDuration    0.2f
	#define EffectStrengthMax 9999.0f
	#define EffectDurationMax 9999.0f
protected:
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Effect)) uint8 defaultEffect;
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Effect)) uint8 defaultEffectImmunity;
private:
	uint8 effect;
	uint8 effectImmunity;
	float effectStrength[static_cast<uint8>(Effect::Length)];
	float effectDuration[static_cast<uint8>(Effect::Length)];
	float effectHit;
	bool  refreshColor = false;
	bool  refreshSpeed = false;
protected:
	virtual void UpdateEffect(float DeltaTime);
public:
	virtual void Damage(AEntity* entity, float value);
	bool         HasEffect   (Effect value);
	virtual void AddEffect   (Effect value, float strength = 1.0f, float duration = EffectDurationMax);
	virtual void RemoveEffect(Effect value);

	float GetEffectStrength(Effect value);
	float GetEffectDuration(Effect value);
	float AdjustEffectStrength(Effect value, float strength);
	float AdjustEffectDuration(Effect value, float duration);
};
