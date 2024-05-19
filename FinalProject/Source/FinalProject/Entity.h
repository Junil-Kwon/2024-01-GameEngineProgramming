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

uint8 GetIndex(Tag tag);
uint8 GetIndex(Effect effect);
template <typename T> uint8 ToInt(T enumerate);
template <typename T> T ToEnum(uint8 integer);

FRotator VectorToRotator(FVector vector);
FVector RotatorToVector(FRotator rotator);



UCLASS(HideCategories = ("Actor Tick", Replication, Rendering, Collision, Actor, Input, LOD, Cooking))
class FINALPROJECT_API AEntity : public ACharacter {
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



	#define MaxFallSpeed      -1024.0f
	#define Particlethreshold  -300.0f
private:
	bool  isFalling = false;
	float fallSpeed = 0.0f;
public:
	virtual void Tick(float DeltaTime) override;



	// Identifier
private:
	Identifier identifier = Identifier::Default;
public:
	Identifier GetIdentifier();



protected:
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* shadowComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* meshComponent;

	UPROPERTY(EditAnywhere, Category = "Entity") State  state = State::None;
	UPROPERTY(EditAnywhere, Category = "Entity") Group  group = Group::None;
	UPROPERTY(EditAnywhere, Category = "Entity") Tag    tag = Tag::None;
	UPROPERTY(EditAnywhere, Category = "Entity") Effect effect         = Effect::None;
	UPROPERTY(EditAnywhere, Category = "Entity") Effect effectImmunity = Effect::None;
	UPROPERTY(EditAnywhere, Category = "Entity") float  speed = 0.0f;

public:
	bool HasTag(Tag value);
	virtual void AddTag(Tag value);
	virtual void RemoveTag(Tag value);

protected:
	TArray<float> effectStrength;
	TArray<float> effectDuration;
public:
	bool HasEffect(Effect value);
	virtual bool AddEffect(Effect value, float strength, float duration);
	virtual void RemoveEffect(Effect value);
protected:
	virtual void UpdateEffect();

protected:
	int32 spriteIndex = 0;
	float spriteDelay = 0.0f;
	bool  spriteXflip = false;
	virtual void UpdateSprite();

protected:
	float hitboxRadius;
	float hitboxHeight;
	FVector GetFootLocation();
};
