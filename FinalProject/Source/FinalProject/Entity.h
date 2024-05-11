#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Entity.generated.h"



UENUM(BlueprintType)
enum class Identifier : uint8 {
	None			= 0,

	// Creatures
	Hero			,

	// Objects

	// Indicator
	Indicator		,

	// Particles
	Dust			,

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



UCLASS()
class FINALPROJECT_API AEntity : public ACharacter {
	GENERATED_BODY()

public:
	AEntity();
protected:
	virtual void BeginPlay() override;

private:
	const float MaxFallSpeed = 1024.0f;
	bool  isFalling = false;
	float fallSpeed = 0.0f;
public:	
	virtual void Tick(float DeltaTime) override;



	// Identifier
private:
	UPROPERTY(EditAnywhere) Identifier identifier = Identifier::None;
	UClass* GetClass(Identifier value);
public:
	AActor* Spawn(Identifier value, FVector location);

	// Hitbox
private:
	float hitboxRadius = 0.0f;
	float hitboxHeight = 0.0f;
public:
	float GetHitboxRadius();
	float GetHitboxHeight();
	bool  SetHitboxRadius(float value);
	bool  SetHitboxHeight(float value);
	bool  SetHitbox(float radius, float height);
	FVector GetFootLocation();

	// Sprite
private:
	UPROPERTY() class UStaticMesh* sprite;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* spriteComponent;
	int32 spriteIndex = 0;
	float spriteScale = 1.0f;
	bool  spriteXflip = false;
protected:
	float spriteDelay = 0.0f;
	int32 GetSpriteIndex();
	float GetSpriteScale();
	bool  GetSpriteXFlip();
	bool  SetSpriteIndex(int32 value);
	bool  SetSpriteScale(float value);
	bool  SetSpriteXFlip(bool  value);
	virtual void UpdateSprite(float DeltaTime);

	// Shadow
private:
	UPROPERTY() class UStaticMesh* shadow;
	UPROPERTY() class UStaticMeshComponent* shadowComponent;



	// State
private:
	UPROPERTY(EditAnywhere) State state = State::Healthy;
public:
	State GetState();
	virtual bool SetState(State value);

	// Group
private:
	UPROPERTY(EditAnywhere) Group group = Group::None;
public:
	Group GetGroup();
	virtual bool SetGroup(Group value);

	// Tag
private:
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Tag)) uint8 tag;
public:
	bool HasTag(Tag value);
	virtual bool AddTag   (Tag value);
	virtual bool RemoveTag(Tag value);

	// Effect
private:
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Effect)) uint8 effect = 0;
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Effect)) uint8 effectImmunity = 0;
	TArray<float> effectStrength;
	TArray<float> effectDuration;
protected:
	uint8 GetIndex(Effect value);
	float GetEffectStrength(uint8 value);
	float GetEffectDuration(uint8 value);
	void  SetEffectStrength(uint8 value, float strength);
	void  SetEffectDuration(uint8 value, float duration);
	virtual void UpdateEffect(float DeltaTime);
public:
	bool HasEffect(Effect value);
	virtual bool AddEffect   (Effect value, float strength, float duration);
	virtual bool RemoveEffect(Effect value);

	// Movement
protected:
	UPROPERTY(EditAnywhere) float speed = 400.0f;
	FVector direction = FVector::ZeroVector;
	//float GetSpeed();
};
