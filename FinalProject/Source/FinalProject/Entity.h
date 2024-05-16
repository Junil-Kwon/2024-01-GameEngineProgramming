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
	Piercing		= 1 << 1,
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



	// Physics
	#define Gravity            -980.0f
	#define ParticleThreshold  -500.0f
	#define VoidThreshold     -1000.0f
	#define DefaultSpeed        300.0f
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

private:
	UPROPERTY(EditAnywhere) float speed = DefaultSpeed;
	FVector direction = FVector::ZeroVector;
	FVector velocity  = FVector::ZeroVector;
	FVector force     = FVector::ZeroVector;
public:
	FVector GetDirection();
	void    SetDirection(FVector value);
	void    AddForce    (FVector value);



	// Identifier
private:
	Identifier identifier = Identifier::Default;
public:
	Identifier GetIdentifier();

	// Hitbox
	#define DefaultHitboxRadius  50.0f
	#define DefaultHitboxHeight 100.0f
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
	void  SetHitboxRadius(float value = DefaultHitboxRadius);
	void  SetHitboxHeight(float value = DefaultHitboxHeight);
	void  SetHitbox(float radius = DefaultHitboxRadius, float height = DefaultHitboxHeight);
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
private:
	UPROPERTY(EditAnywhere) Group group = Group::None;
public:
	Group        GetGroup();
	virtual void SetGroup(Group value);

	// Tag
private:
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Tag)) uint8 tag;
public:
	bool         HasTag   (Tag value);
	virtual bool AddTag   (Tag value);
	virtual bool RemoveTag(Tag value);

	// Effect
	#define EffectMaxStrength 9999.0f
	#define EffectMaxDuration 9999.0f
private:
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Effect)) uint8 effect = 0;
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Effect)) uint8 effectImmunity = 0;
	TArray<float> effectStrength;
	TArray<float> effectDuration;
protected:
	uint8 GetIndex(Effect value);
	float GetEffectStrength(uint8 value);
	float GetEffectDuration(uint8 value);
	void  AdjustEffectStrength(uint8 value, float strength);
	void  AdjustEffectDuration(uint8 value, float duration);
	virtual bool UpdateEffect(float DeltaTime);
public:
	bool         HasEffect   (Effect value);
	virtual bool AddEffect   (Effect value, float strength, float duration = EffectMaxDuration);
	virtual bool RemoveEffect(Effect value);
};
