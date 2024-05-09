#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Entity.generated.h"



UENUM(BlueprintType)
enum class State : uint8 {
	None			= 0,
	Healthy			,
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

UENUM(BlueprintType, Meta = (BitFlags))
enum class Tag : uint8 {
	None			= 0,
	Floating		= 1 << 0,
	Piercing		= 1 << 1,
	Invulnerability	= 1 << 2,
	Interactability	= 1 << 3,
	Player			= 1 << 4,
	Leader			= 1 << 5,
	Max				= 6 UMETA(Hidden),
};

UENUM(BlueprintType, Meta = (BitFlags))
enum class Effect : uint8 {
	None			= 0,
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

uint8 GetIndex(Tag tag);
uint8 GetIndex(Effect effect);
template <typename T> uint8 ToInt(T enumerate);
template <typename T> T ToEnum(uint8 integer);

FRotator VectorToRotator(FVector vector);
FVector RotatorToVector(FRotator rotator);



UCLASS()
class FINALPROJECT_API AEntity : public ACharacter {
	GENERATED_BODY()

public:
	AEntity();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool HasTag(Tag value);
	virtual void AddTag(Tag value);
	virtual void RemoveTag(Tag value);

	bool HasEffect(Effect value);
	virtual bool AddEffect(Effect value, float strength, float duration);
	virtual void RemoveEffect(Effect value);

protected:
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* shadowComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* meshComponent;

	UPROPERTY(EditAnywhere, Category = "Entity") State  state = State::None;
	UPROPERTY(EditAnywhere, Category = "Entity") Group  group = Group::None;
	UPROPERTY(EditAnywhere, Category = "Entity") Tag    tag = Tag::None;
	UPROPERTY(EditAnywhere, Category = "Entity") Effect effect = Effect::None;
	UPROPERTY(EditAnywhere, Category = "Entity") Effect effectImmunity = Effect::None;
	UPROPERTY(EditAnywhere, Category = "Entity") float  speed = 0.0f;
};
