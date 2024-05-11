#include "Entity.h"
#include "Particle.h"
#include "Indicator.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/StaticMesh.h"



template <typename T> T     ToEnum(uint8 value) { return static_cast<T    >(value); }
template <typename T> uint8 ToInt (T     value) { return static_cast<uint8>(value); }
FRotator VectorToRotator(FVector  value) {
	float degree = 90.0f - FMath::RadiansToDegrees(FMath::Atan2(value.X, value.Y));
	return FRotator(0.0f, degree, 0.0f);
}
FVector  RotatorToVector(FRotator value) {
	float radian = FMath::DegreesToRadians(value.Yaw);
	return FVector(FMath::Cos(radian), FMath::Sin(radian), 0.0f);
}



AEntity::AEntity() {
 	PrimaryActorTick.bCanEverTick = true;
	GetClass(Identifier::None);

	const TCHAR* Plane  = TEXT("/Engine/BasicShapes/Plane.Plane"  );
	const TCHAR* Sphere = TEXT("/Engine/BasicShapes/Sphere.Sphere");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sprite(Plane );
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Shadow(Sphere);
	sprite = Sprite.Object;
	shadow = Shadow.Object;

	GetCapsuleComponent()->InitCapsuleSize(50.0f, 50.0f);

	spriteComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	spriteComponent->SetStaticMesh(sprite);
	spriteComponent->SetWorldRotation(FRotator(0.0f, 90.0f, 41.409618f));
	spriteComponent->SetWorldScale3D(FVector(1.28f, 1.28f, 1.28f));
	spriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	spriteComponent->SetupAttachment(RootComponent);

	shadowComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShadowComponent"));
	shadowComponent->SetStaticMesh(shadow);
	shadowComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	shadowComponent->SetVisibility(false);
	shadowComponent->SetupAttachment(RootComponent);

	GetCharacterMovement()->JumpZVelocity = 800.0f;
	GetCharacterMovement()->GravityScale = HasTag(Tag::Floating) ? 0.0f : 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = speed;
	GetCharacterMovement()->MaxAcceleration = speed * 100;
}



void AEntity::BeginPlay() {
	Super::BeginPlay();
	
	hitboxRadius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	hitboxHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() * 2.0f;
	ECollisionEnabled::Type type = ECollisionEnabled::QueryAndPhysics;
	if (!hitboxRadius || !hitboxHeight) ECollisionEnabled::QueryOnly;
	GetCapsuleComponent()->SetCollisionEnabled(type);

	spriteScale = spriteComponent->GetRelativeScale3D().X / 1.28f;

	FVector scale = FVector(hitboxRadius * 0.02f, hitboxRadius * 0.02f, hitboxHeight * 0.01f);
	shadowComponent->SetWorldScale3D(scale);
	shadowComponent->SetVisibility(true);
	shadowComponent->bCastHiddenShadow = true;
	shadowComponent->bHiddenInGame = true;

	State stateTemp = state;
	state = State::Healthy;
	SetState(stateTemp);

	Group groupTemp = group;
	group = Group::None;
	SetGroup(groupTemp);

	uint8 tagTemp = tag;
	tag = 0;
	for (uint8 i = 0; i < ToInt(Tag::Max); i++) {
		Tag value = ToEnum<Tag>(1 << i);
		if ((tagTemp & (1 << i)) == 0) continue;
		AddTag(value);
	}

	uint8 effectTemp = effect;
	effect = 0;
	effectStrength.Init(0.0f, static_cast<uint8>(Effect::Max));
	effectDuration.Init(0.0f, static_cast<uint8>(Effect::Max));
	float strength = 0.0f;
	float duration = 0.0f;
	for (uint8 i = 0; i < ToInt(Effect::Max); i++) {
		Effect value = ToEnum<Effect>(1 << i);
		if ((effectTemp & (1 << i)) == 0 || (effectImmunity & (1 << i)) != 0) continue;
		switch (value) {
		case Effect::HealthBoost: strength = 0.0f, duration = 9999.9f; break;
		case Effect::DamageBoost: strength = 0.0f, duration = 9999.9f; break;
		case Effect::Resistance:  strength = 0.0f, duration = 9999.9f; break;
		case Effect::Speed:       strength = 0.0f, duration = 9999.9f; break;
		case Effect::Burn:        strength = 0.0f, duration = 9999.9f; break;
		case Effect::Stun:        strength = 0.0f, duration = 9999.9f; break;
		case Effect::Freeze:      strength = 0.0f, duration = 9999.9f; break;
		case Effect::Slowness:    strength = 0.0f, duration = 9999.9f; break;
		}
		AddEffect(value, strength, duration);
	}
}



void AEntity::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsFalling()) {
		if (!isFalling) isFalling = true;
		fallSpeed = -GetCharacterMovement()->Velocity.Z;
		if (MaxFallSpeed < fallSpeed) GetCharacterMovement()->Velocity.Z = -MaxFallSpeed;
	}
	else if (isFalling) {
		if (500.0f < fallSpeed) {
			Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, -hitboxRadius *  0.75f, 0.0f));
			Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, -hitboxRadius * -0.75f, 0.0f));
		}
		isFalling = false;
		fallSpeed = 0.0f;
	}

	UpdateSprite(DeltaTime);
	UpdateEffect(DeltaTime);
}



UClass* AEntity::GetClass(Identifier value) {
	static TArray<TTuple<Identifier, UClass*>> prefab;
	static bool initialized = false;
	if (!initialized) {
		initialized = true;

		const TCHAR* BP_Indicator = TEXT("/Game/Blueprints/BP_Indicator.BP_Indicator_C");
		static ConstructorHelpers::FClassFinder <AIndicator> Indicator(BP_Indicator);
		prefab.Add(TTuple<Identifier, UClass*>(Identifier::Indicator, Indicator.Class));

		const TCHAR* BP_Particle_Dust = TEXT("/Game/Blueprints/BP_Particle_Dust.BP_Particle_Dust_C");
		static ConstructorHelpers::FClassFinder <AParticle> Dust(BP_Particle_Dust);
		prefab.Add(TTuple<Identifier, UClass*>(Identifier::Dust, Dust.Class));

		return nullptr;
	}
	for (int32 i = 0; i < prefab.Num(); i++) if (prefab[i].Key == value) return prefab[i].Value;
	return nullptr;
}
AActor* AEntity::Spawn(Identifier value, FVector location = FVector::ZeroVector) {
	return GetWorld()->SpawnActor<AActor>(GetClass(value), location, FRotator::ZeroRotator);
}



float AEntity::GetHitboxRadius() { return hitboxRadius; }
float AEntity::GetHitboxHeight() { return hitboxHeight; }
bool  AEntity::SetHitboxRadius(float value) {
	if (hitboxRadius == value) return false;
	hitboxRadius = value;
	GetCapsuleComponent()->SetCapsuleSize(value, hitboxHeight);
	ECollisionEnabled::Type type = ECollisionEnabled::QueryAndPhysics;
	if (!hitboxRadius || !hitboxHeight) ECollisionEnabled::QueryOnly;
	GetCapsuleComponent()->SetCollisionEnabled(type);
	return true;
}
bool  AEntity::SetHitboxHeight(float value) {
	if (hitboxHeight == value) return false;
	hitboxHeight = value;
	GetCapsuleComponent()->SetCapsuleSize(hitboxRadius, value);
	ECollisionEnabled::Type type = ECollisionEnabled::QueryAndPhysics;
	if (!hitboxRadius || !hitboxHeight) ECollisionEnabled::QueryOnly;
	GetCapsuleComponent()->SetCollisionEnabled(type);
	return true;
}
bool  AEntity::SetHitbox(float radius, float height) {
	if (hitboxRadius == radius && hitboxHeight == height) return false;
	hitboxRadius = radius;
	hitboxHeight = height;
	GetCapsuleComponent()->SetCapsuleSize(radius, height);
	ECollisionEnabled::Type type = ECollisionEnabled::QueryAndPhysics;
	if (!hitboxRadius || !hitboxHeight) ECollisionEnabled::QueryOnly;
	GetCapsuleComponent()->SetCollisionEnabled(type);
	return true;
}
FVector AEntity::GetFootLocation() {
	return GetActorLocation() + FVector(-hitboxHeight * 0.5f, 0.0f, -hitboxHeight * 0.5f + 64.0f);
}



int32 AEntity::GetSpriteIndex() { return spriteIndex; }
float AEntity::GetSpriteScale() { return spriteScale; }
bool  AEntity::GetSpriteXFlip() { return spriteXflip; }
bool  AEntity::SetSpriteIndex(int32 value) {
	if (spriteIndex == value) return false;
	spriteIndex = value;
	spriteComponent->SetScalarParameterValueOnMaterials(TEXT("Index"), value);
	return true;
}
bool AEntity::SetSpriteScale(float value) {
	if (spriteScale == value) return false;
	spriteScale = value;
	spriteComponent->SetRelativeScale3D(FVector(1.28f, 1.28f, 1.28f) * spriteScale);
	return true;
}
bool  AEntity::SetSpriteXFlip(bool value) {
	if (spriteXflip == value) return false;
	spriteXflip = value;
	spriteComponent->SetScalarParameterValueOnMaterials(TEXT("XFlip"), value ? 1.0f : 0.0f);
	return true;
}
void  AEntity::UpdateSprite(float DeltaTime) {
	spriteDelay += DeltaTime;
}



State AEntity::GetState() {
	return state;
}
bool AEntity::SetState(State value) {
	if (state == value) return false;
	state = value;
	switch (value) {
	case State::Healthy: break;
	case State::Dead:    break;
	}
	return true;
}



Group AEntity::GetGroup() {
	return group;
}
bool AEntity::SetGroup(Group value) {
	if (group == value) return false;
	group = value;
	return true;
}



bool AEntity::HasTag(Tag value) {
	return tag & ToInt(value);
}
bool AEntity::AddTag(Tag value) {
	if (HasTag(value)) return false;
	tag |= ToInt(value);
	switch (value) {
	case Tag::Floating:        break;
	case Tag::Piercing:        break;
	case Tag::Invulnerability: break;
	case Tag::Interactability: break;
	case Tag::Player:          break;
	case Tag::Leader:          break;
	}
	return true;
}
bool AEntity::RemoveTag(Tag value) {
	if (!HasTag(value)) return false;
	tag &= ~ToInt(value);
	switch (value) {
	case Tag::Floating:        break;
	case Tag::Piercing:        break;
	case Tag::Invulnerability: break;
	case Tag::Interactability: break;
	case Tag::Player:          break;
	case Tag::Leader:          break;
	}
	return true;
}



bool AEntity::HasEffect(Effect value) {
	return (ToInt(effect) & ToInt(value)) != 0;
}
bool AEntity::AddEffect(Effect value, float strength = 1.0f, float duration = 1.0f) {
	if ((ToInt(effectImmunity) & ToInt(value)) != 0) return false;
	uint8 i = GetIndex(value);
	effect |= ToInt(value);
	effectStrength[i] = strength;
	effectDuration[i] = duration;
	switch (value) {
	case Effect::HealthBoost: break;
	case Effect::DamageBoost: break;
	case Effect::Resistance:  break;
	case Effect::Speed:       break;
	case Effect::Burn:        break;
	case Effect::Stun:        break;
	case Effect::Freeze:      break;
	case Effect::Slowness:    break;
	}
	return true;
}
bool AEntity::RemoveEffect(Effect value) {
	if (!HasEffect(value)) return false;
	uint8 i = GetIndex(value);
	effect &= ~ToInt(value);
	effectStrength[i] = 0;
	effectDuration[i] = 0;
	switch (value) {
	case Effect::HealthBoost: break;
	case Effect::DamageBoost: break;
	case Effect::Resistance:  break;
	case Effect::Speed:       break;
	case Effect::Burn:        break;
	case Effect::Stun:        break;
	case Effect::Freeze:      break;
	case Effect::Slowness:    break;
	}
	return true;
}
uint8 AEntity::GetIndex(Effect value) {
	for (int i = 0; i < ToInt(Effect::Max); i++) if (ToInt(value) & (1 << i)) return i;
	return 0;
}
float AEntity::GetEffectStrength(uint8 value) { return effectStrength[value]; }
float AEntity::GetEffectDuration(uint8 value) { return effectDuration[value]; }
void  AEntity::SetEffectStrength(uint8 value, float strength) { effectStrength[value] = strength; }
void  AEntity::SetEffectDuration(uint8 value, float duration) { effectDuration[value] = duration; }
void  AEntity::UpdateEffect(float DeltaTime) {
	for (uint8 i = 0; i < ToInt(Effect::Max); i++) {
		Effect value = ToEnum<Effect>(1 << i);
		if (HasEffect(value) && (effectDuration[i] -= DeltaTime) <= 0) {
			effectStrength[i] = 0;
			effectDuration[i] = 0;
			RemoveEffect(value);
		}
	}
}
