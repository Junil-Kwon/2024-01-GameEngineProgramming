#include "Entity.h"
#include "Ghost.h"
#include "Creature.h"
#include "Interactor.h"
#include "Particle.h"

#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Engine/Font.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"





// =============================================================================================================
// -
// =============================================================================================================

bool AEntity::operator==(const AEntity& other) const { return this == &other; }

FRotator AEntity::ToRotator(FVector  value) {
	float degree = 90.0f - FMath::RadiansToDegrees(FMath::Atan2(value.X, value.Y));
	return FRotator(0.0f, degree, 0.0f);
}
FVector  AEntity::ToVector(FRotator value) {
	float radian = FMath::DegreesToRadians(value.Yaw);
	return FVector(FMath::Cos(radian), FMath::Sin(radian), 0.0f);
}
FVector AEntity::RotateVector(FVector value) {
	const FRotator rotation = FRotator(-48.590382f, 0.0f, 0.0f);
	return rotation.RotateVector(FVector(value.X, value.Y, value.Z));
}



template<typename TEnum> uint8 AEntity::GetIndex(TEnum value) {
	static uint8 list[255] = { 0, };
	uint8 index = static_cast<uint8>(value);
	if (!std::is_same_v<TEnum, Tag> && !std::is_same_v<TEnum, Effect>) return index;
	if (!list[1]) for (uint8 i = 0; i < 8; i++) list[uint8(1 << i)] = i;
	return list[index];
}
template<typename TEnum> FString AEntity::ToFString(TEnum value) {
	return StaticEnum<TEnum>()->GetNameStringByValue(static_cast<uint8>(value));
}
template<typename TEnum> TEnum AEntity::ToEnum(FString value) {
	int64 index = StaticEnum<TEnum>()->GetValueByName(FName(*value));
	return static_cast<TEnum>(index == INDEX_NONE ? 0 : index);
}



UStaticMesh* AEntity::GetStaticMesh(MeshType value) {
	static UStaticMesh* uStaticMesh[static_cast<uint8>(MeshType::Length)] = { nullptr, };
	uint8 index = static_cast<uint8>(value);
	if (uStaticMesh[index] == nullptr) {
		FString name = ToFString(value);
		FString path = "/Engine/BasicShapes/" + name + "." + name;
		uStaticMesh[index] = LoadObject<UStaticMesh>(nullptr, *path);
	}
	return uStaticMesh[index];
}

UMaterialInstanceDynamic* AEntity::GetMaterialInstanceDynamic(Identifier value) {
	static UTexture* uTexture[static_cast<uint8>(Identifier::Length)] = { nullptr, };
	uint8 index = static_cast<uint8>(value);
	if (uTexture[index] == nullptr) {
		FString name = ToFString(value);
		FString path = "/Game/Textures/" + name + "." + name;
		uTexture[index] = LoadObject<UTexture>(nullptr, *path);
	}
	static UMaterialInstance* uInstance = nullptr;
	if (uInstance == nullptr) {
		FString path = "/Game/Textures/MaterialInstance.MaterialInstance";
		uInstance = LoadObject<UMaterialInstance>(nullptr, *path);
	}
	UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(uInstance, this);
	material->SetTextureParameterValue(TEXT("Texture"), uTexture[index]);
	return material;
}

UFont* AEntity::GetFont(FontType value) {
	static UFont* uFont[static_cast<uint8>(FontType::Length)] = { nullptr, };
	uint8 index = static_cast<uint8>(value);
	if (uFont[index] == nullptr) {
		FString name = ToFString(value);
		FString path = "/Game/Fonts/" + name + "-Font." + name + "-Font";
		uFont[index] = LoadObject<UFont>(nullptr, *path);
	}
	return uFont[index];
}
UMaterial* AEntity::GetFontMaterial(FontType value) {
	static UMaterial* uMaterial[static_cast<uint8>(FontType::Length)] = { nullptr, };
	uint8 index = static_cast<uint8>(value);
	if (uMaterial[index] == nullptr) {
		FString name = ToFString(value);
		FString path = "/Game/Fonts/" + name + "-Material." + name + "-Material";
		uMaterial[index] = LoadObject<UMaterial>(nullptr, *path);
	}
	return uMaterial[index];
}

AEntity* AEntity::Spawn(Identifier value, FVector location) {
	static UClass* uClass[static_cast<uint8>(Identifier::Length)] = { nullptr, };
	uint8 index = static_cast<uint8>(value);
	if (uClass[index] == nullptr) {
		FString name = ToFString(value);
		FString path = "/Game/Blueprints/BP_" + name + ".BP_" + name + "_C";
		uClass[index] = LoadObject<UClass>(nullptr, *path);
	}
	FActorSpawnParameters parameter;
	parameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return GetWorld()->SpawnActor<AEntity>(uClass[index], location, FRotator::ZeroRotator, parameter);
}





// =============================================================================================================
// Initialization
// =============================================================================================================

AEntity::AEntity() {
 	PrimaryActorTick.bCanEverTick = true;

	hitboxComponent = GetCapsuleComponent();
	hitboxComponent->InitCapsuleSize(defaultHitboxRadius, defaultHitboxHeight * 0.5f);
	hitboxComponent->SetCollisionProfileName(TEXT("Entity"));
	SetRootComponent(hitboxComponent);

	spriteComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sprite"));
	spriteComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 41.409618f));
	spriteComponent->SetRelativeScale3D(FVector(1.28f, 1.28f, 1.28f));
	spriteComponent->SetStaticMesh(GetStaticMesh(MeshType::Plane));
	spriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	spriteComponent->SetupAttachment(RootComponent);

	shadowComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shadow"));
	shadowComponent->SetStaticMesh(GetStaticMesh(MeshType::Sphere));
	shadowComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	shadowComponent->SetVisibility(false);
	shadowComponent->SetupAttachment(RootComponent);
}
void AEntity::BeginPlay() {
	Super::BeginPlay();
	
	FString name = GetName();
	int64 BP = name.Find(TEXT("_"), ESearchCase::IgnoreCase, ESearchDir::FromStart, 0);
	int64 C0 = name.Find(TEXT("_"), ESearchCase::IgnoreCase, ESearchDir::FromStart, BP + 1);
	FString edit = name.Mid(BP + 1, C0 - BP - 1);
	for (int64 i = edit.Len() - 1; -1 < i; i--) if ('0' <= edit[i] && edit[i] <= '9') edit.RemoveAt(i, 1);
	identifier = ToEnum<Identifier>(edit);

	SetHitbox(defaultHitboxRadius, defaultHitboxHeight);
	handLocation = FVector(0.0f, defaultHandLocation.X, defaultHandLocation.Y);

	spriteComponent->SetMaterial(0, GetMaterialInstanceDynamic(identifier));

	shadowComponent->SetVisibility(true);
	shadowComponent->bCastHiddenShadow = true;
	shadowComponent->bHiddenInGame = true;

	speed = defaultSpeed;
	GetCharacterMovement()->MaxWalkSpeed    = speed;
	GetCharacterMovement()->MaxAcceleration = speed * 100;
	GetCharacterMovement()->JumpZVelocity   = 800.0f;
	GetCharacterMovement()->GravityScale    = DefaultGravityScale;
	SetLookDirection(FVector(0.0f, 1.0f, 0.0f));

	SetGroup(defaultGroup);
	for (uint8 i = 0; i < static_cast<uint8>(Tag::Length); i++) {
		if (defaultTag & (1 << i)) AddTag(static_cast<Tag>(1 << i));
	}
	effectImmunity = defaultEffectImmunity;
	for (uint8 i = 0; i < static_cast<uint8>(Effect::Length); i++) {
		effectStrength[i] = 0;
		effectDuration[i] = 0;
		if (defaultEffect & (1 << i)) AddEffect(static_cast<Effect>(1 << i));
	}
}
void AEntity::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	if (HasTag(Tag::Interactability)) RemoveInteractor();
	if (HasTag(Tag::Player)) GetGhost()->OnPlayerDestroyed();
}

// =============================================================================================================
// Update
// =============================================================================================================

void AEntity::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsFalling()) {
		if (!isFalling) isFalling = true;
		fallSpeed = GetCharacterMovement()->Velocity.Z;
		if (fallSpeed < FallSpeedMax) GetCharacterMovement()->Velocity.Z = FallSpeedMax;
		if (GetFootLocation().Z < VoidZAxis) Destroy();
	}
	else if (isFalling) {
		if (fallSpeed < DustThreshold) {
			Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, -hitboxRadius *  0.75f, 0.0f));
			Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, -hitboxRadius * -0.75f, 0.0f));
		}
		isFalling = false;
		fallSpeed = 0.0f;
	}

	float multiplier = (HasEffect(Effect::Stun) ? 0.0f : 1.0f) * (1.0f - GetEffectStrength(Effect::Freeze));
	if (0.0f < multiplier) UpdateInputs(DeltaTime * multiplier);
	if (0.0f < multiplier) UpdateAction(DeltaTime * multiplier);
	UpdateEffect(DeltaTime);
}

FVector AEntity::GetMoveDirection() { return moveDirection; }
FVector AEntity::GetLookDirection() { return lookDirection; }
void AEntity::SetMoveDirection(FVector value) {
	value.Normalize();
	moveDirection = value;
}
void AEntity::SetLookDirection(FVector value) {
	if (value.IsZero()) return;
	value.Normalize();
	lookDirection = value;
	if (lookDirection.Y != 0 && GetSpriteXFlip() != (lookDirection.Y < 0.0f)) {
		SetSpriteXFlip(nullptr, lookDirection.Y < 0.0f);
	}
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void AEntity::OnHitboxChanged() {
	hitboxComponent->SetCapsuleSize(hitboxRadius, hitboxHeight * 0.5f);
	FVector shadowScale = FVector(hitboxRadius * 2, hitboxRadius * 2, hitboxHeight) * 0.01f;
	shadowScale = shadowScale - FVector::OneVector * 0.2f;
	if (shadowScale.X <= 0.0f || shadowScale.Z <= 0.0f) shadowScale = FVector::ZeroVector;
	shadowComponent->SetRelativeScale3D(shadowScale);
}
float AEntity::GetHitboxRadius() { return hitboxRadius; }
float AEntity::GetHitboxHeight() { return hitboxHeight; }
void  AEntity::SetHitboxRadius(float value) {
	hitboxRadius = value;
	OnHitboxChanged();
}
void  AEntity::SetHitboxHeight(float value) {
	hitboxHeight = value;
	OnHitboxChanged();
}
void  AEntity::SetHitbox(float radius, float height) {
	hitboxRadius = radius;
	hitboxHeight = height;
	OnHitboxChanged();
}
void  AEntity::SetCollisionProfileName(FName value) {
	hitboxComponent->SetCollisionProfileName(value);
}
FVector AEntity::GetFootLocation() {
	return GetActorLocation() + RotateVector(FVector(-24.0f, 0.0f, -hitboxHeight * 0.5f));
}
FVector AEntity::GetHandLocation() {
	FVector hand = FVector(-24.0f, handLocation.Y * (!GetSpriteXFlip() ? 1.0f : -1.0f), handLocation.Z);
	return GetActorLocation() + RotateVector(hand);
}

// =============================================================================================================
// Sprite
// =============================================================================================================

int32   AEntity::GetSpriteIndex() { return spriteIndex; }
bool    AEntity::GetSpriteXFlip() { return spriteXFlip; }
FVector AEntity::GetSpriteColor() { return spriteColor; }
float   AEntity::GetSpriteIntensity() { return spriteIntensity; }

void AEntity::SetSpriteIndex(UStaticMeshComponent* component, int32 value) {
	if (component == nullptr) {
		if (value == spriteIndex) return;
		component = spriteComponent;
		spriteIndex = value;
	}
	component->SetScalarParameterValueOnMaterials(TEXT("Index"), value);
}
void AEntity::SetSpriteXFlip(UStaticMeshComponent* component, bool value) {
	if (component == nullptr) {
		if (value == spriteXFlip) return;
		component = spriteComponent;
		spriteXFlip = value;
	}
	component->SetScalarParameterValueOnMaterials(TEXT("XFlip"), value ? 1.0f : 0.0f);
}
void AEntity::SetSpriteColor(UStaticMeshComponent* component, FVector value) {
	if (component == nullptr) {
		if (value == spriteColor) return;
		component = spriteComponent;
		spriteColor = value;
	}
	component->SetVectorParameterValueOnMaterials(TEXT("Color"), value);
}
void AEntity::SetSpriteIntensity(UStaticMeshComponent* component, float value) {
	if (component == nullptr) {
		if (value == spriteIntensity) return;
		component = spriteComponent;
		spriteIntensity = value;
	}
	component->SetScalarParameterValueOnMaterials(TEXT("Intensity"), value);
}

// =============================================================================================================
// Interactor
// =============================================================================================================

void AEntity::CreateInteractor() {
	if (interactor != nullptr) return;
	interactor = static_cast<AInteractor*>(Spawn(Identifier::Interactor));
	interactor->OnInteract(this);
}
void AEntity::RemoveInteractor() {
	if (interactor == nullptr) return;
	interactor->Destroy();
	interactor = nullptr;
}

AInteractor* AEntity::GetInteractor() { return interactor; }





// =============================================================================================================
// Input
// =============================================================================================================

AGhost* AEntity::GetGhost() {
	if (ghost == nullptr) ghost = Cast<AGhost>(UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0));
	return ghost;
}

bool AEntity::GetInput(Action value) {
	if (!GetGhost()) return false;
	return GetGhost()->GetInput(value);
	return false;
}
FVector AEntity::GetInputDirection() {
	if (!GetGhost()) return FVector::ZeroVector;
	return GetGhost()->GetInputDirection();
}

// =============================================================================================================
// Action
// =============================================================================================================

Action AEntity::GetAction() {
	return action;
}
bool AEntity::SetAction(Action value) {
	if (!VerifyAction(value) || actionCooldown[static_cast<uint8>(value)]) return false;
	action = value;
	actionDelay = 0.0f;
	return true;
}
float AEntity::GetActionCooldown(Action value) {
	return actionCooldown[static_cast<uint8>(value)];
}
void AEntity::SetActionCooldown(Action value, float cooldown) {
	actionCooldown[static_cast<uint8>(value)] = FMath::Max(cooldown, 0.0f);
}
bool AEntity::VerifyAction(Action value) {
	return false;
}
bool AEntity::UpdateInputs(float DeltaTime) {
	if (HasTag(Tag::Player)) {
		SetMoveDirection(GetInputDirection());
		if (action == Action::Idle && !GetMoveDirection().IsZero()) SetAction(Action::Move);
		if (action == Action::Move &&  GetMoveDirection().IsZero()) SetAction(Action::Idle);
		for (uint8 i = 0; i < static_cast<uint8>(Action::Length); i++) {
			Action index = static_cast<Action>(i);
			if (GetInput(index)) SetAction(index);
		}
		return false;
	}
	return true;
}
bool AEntity::UpdateAction(float DeltaTime) {
	actionDelay += DeltaTime;
	for (uint8 i = 0; i < static_cast<uint8>(Action::Length); i++) {
		actionCooldown[i] = FMath::Max(actionCooldown[i] - DeltaTime, 0.0f);
	}
	return true;
}

bool AEntity::OnInteract(AEntity* entity) {
	return (entity != nullptr);
}





// =============================================================================================================
// Identifier
// =============================================================================================================

Identifier AEntity::GetIdentifier() { return identifier; }

// =============================================================================================================
// Group
// =============================================================================================================

Group AEntity::GetGroup() { return group; }
void AEntity::SetGroup(Group value) { group = value; }

// =============================================================================================================
// Tag
// =============================================================================================================

bool AEntity::HasTag(Tag value) {
	return tag & static_cast<uint8>(value);
}
bool AEntity::AddTag(Tag value) {
	if (HasTag(value)) return false;
	tag |= static_cast<uint8>(value);
	switch (value) {
	case Tag::Floating:        GetCharacterMovement()->GravityScale = 0.0f; break;
	case Tag::Piercing:        hitboxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); break;
	case Tag::Invulnerability: break;
	case Tag::Interactability: CreateInteractor(); break;
	case Tag::Collectable:     break;
	case Tag::Player:          GetGhost()->SetPlayer(this); break;
	case Tag::Leader:          break;
	}
	return true;
}
bool AEntity::RemoveTag(Tag value) {
	if (!HasTag(value)) return false;
	tag &= ~static_cast<uint8>(value);
	switch (value) {
	case Tag::Floating:        GetCharacterMovement()->GravityScale = DefaultGravityScale; break;
	case Tag::Piercing:        hitboxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); break;
	case Tag::Invulnerability: break;
	case Tag::Interactability: RemoveInteractor(); break;
	case Tag::Collectable:     break;
	case Tag::Player:          break;
	case Tag::Leader:          break;
	}
	return true;
}

// =============================================================================================================
// Effect
// =============================================================================================================

void AEntity::Hit(float value) { hit = value; }
bool AEntity::UpdateEffect(float DeltaTime) {
	for (uint8 i = 0; i < static_cast<uint8>(Effect::Length); i++) {
		Effect value = static_cast<Effect>(1 << i);
		if (!HasEffect(value)) continue;
		float duration = AdjustEffectDuration(value, -DeltaTime);
		switch (value) {
		case Effect::Burn:
			if (int32(duration * 4) != int32((duration - DeltaTime) * 4)) {
				AParticle* particle = static_cast<AParticle*>(Spawn(Identifier::Flame));
				FVector location = FVector::ZeroVector;
				float angle = FMath::RandRange(-0.2f * PI, 1.2f * PI);
				location.X = -24.0f;
				location.Y = hitboxRadius * FMath::Cos(angle) * FMath::RandRange(0.6f, 0.8f);
				location.Z = hitboxHeight * FMath::Sin(angle) * FMath::RandRange(0.3f, 0.4f);
				particle->SetActorLocation(GetActorLocation() + RotateVector(location));
				particle->OnInteract(this);
			}
			break;
		}
		if (duration == 0.0f) {
			switch (value) {
			case Effect::HealthBoost:
			case Effect::DamageBoost:
				if (AdjustEffectStrength(value, -DeltaTime) == 0.0f) RemoveEffect(value);
				break;
			case Effect::Resistance:
			case Effect::Burn:
			case Effect::Freeze:
				if (AdjustEffectStrength(value, -DeltaTime * 0.5f) == 0.0f) RemoveEffect(value);
				break;
			case Effect::Speed:
			case Effect::Stun:
			default:
				RemoveEffect(value);
				break;
			}
		}
	}
	if (updateColor || FMath::Max(0.0f, hit -= DeltaTime)) {
		updateColor = false;
		FVector color = FVector(1.0f, 1.0f, 1.0f);
		float intensity = FMath::Clamp(hit * 5.0f, 0.0f, 1.0f);
		if (HasEffect(Effect::Burn)) {
			color += FVector(1.0f, 0.0f, 0.0f) * GetEffectStrength(Effect::Burn) * 8.0f;
			intensity += GetEffectStrength(Effect::Burn);
		}
		if (HasEffect(Effect::Stun)) {
			intensity += 1.0f;
		}
		if (HasEffect(Effect::Freeze)) {
			color += FVector(0.0f, 0.3f, 1.0f) * GetEffectStrength(Effect::Freeze) * 8.0f;
		}
		SetSpriteColor(nullptr, color / color.GetMax());
		SetSpriteIntensity(nullptr, intensity);
	}
	if (updateSpeed) {
		float i = 1.0f;
		if (HasEffect(Effect::Speed )) i *= 1.0f + GetEffectStrength(Effect::Speed);
		if (HasEffect(Effect::Burn  )) i *= 1.0f + FMath::Max(GetEffectStrength(Effect::Burn) * 0.3f, 0.3f);
		if (HasEffect(Effect::Freeze)) i *= 1.0f - GetEffectStrength(Effect::Freeze);
		GetCharacterMovement()->MaxWalkSpeed    = speed * i;
		GetCharacterMovement()->MaxAcceleration = speed * i * 100;
	}
	return true;
}

bool AEntity::HasEffect(Effect value) {
	return static_cast<uint8>(effect) & static_cast<uint8>(value);
}
bool AEntity::AddEffect(Effect value, float strength, float duration) {
	if (static_cast<uint8>(effectImmunity) & static_cast<uint8>(value)) return false;
	effect |= static_cast<uint8>(value);
	AdjustEffectStrength(value, strength);
	AdjustEffectDuration(value, duration);
	return true;
}
bool AEntity::RemoveEffect(Effect value) {
	if (!HasEffect(value)) return false;
	effect &= ~static_cast<uint8>(value);
	AdjustEffectStrength(value, -EffectStrengthMax);
	AdjustEffectDuration(value, -EffectDurationMax);
	return true;
}

float AEntity::GetEffectStrength(Effect value) { return effectStrength[GetIndex(value)]; }
float AEntity::GetEffectDuration(Effect value) { return effectDuration[GetIndex(value)]; }
float AEntity::AdjustEffectStrength(Effect value, float strength) {
	strength = effectStrength[GetIndex(value)] + strength;
	switch (value) {
	case Effect::HealthBoost: strength = FMath::Clamp(strength, 0.0f, EffectStrengthMax); break;
	case Effect::DamageBoost: strength = FMath::Clamp(strength, 0.0f, EffectStrengthMax); break;
	case Effect::Resistance:  strength = FMath::Clamp(strength, 0.0f, 1.0f);              break;
	case Effect::Speed:       strength = FMath::Clamp(strength, 0.0f, EffectStrengthMax); break;
	case Effect::Burn:        strength = FMath::Clamp(strength, 0.0f, EffectStrengthMax); break;
	case Effect::Stun:        strength = FMath::Clamp(strength, 1.0f, 1.0f);              break;
	case Effect::Freeze:      strength = FMath::Clamp(strength, 0.0f, 1.0f);              break;
	}
	switch (value) {
	case Effect::HealthBoost: break;
	case Effect::DamageBoost: break;
	case Effect::Resistance:  break;
	case Effect::Speed:       updateSpeed = true; break;
	case Effect::Burn:        updateColor = true; updateSpeed = true; break;
	case Effect::Stun:        updateColor = true; break;
	case Effect::Freeze:      updateColor = true; updateSpeed = true; break;
	}
	effectStrength[GetIndex(value)] = strength;
	return strength;
}
float AEntity::AdjustEffectDuration(Effect value, float duration) {
	duration = effectDuration[GetIndex(value)] + duration;
	switch (value) {
	case Effect::HealthBoost: duration = FMath::Clamp(duration, 0.0f, EffectDurationMax); break;
	case Effect::DamageBoost: duration = FMath::Clamp(duration, 0.0f, EffectDurationMax); break;
	case Effect::Resistance:  duration = FMath::Clamp(duration, 0.0f, EffectDurationMax); break;
	case Effect::Speed:       duration = FMath::Clamp(duration, 0.0f, EffectDurationMax); break;
	case Effect::Burn:        duration = FMath::Clamp(duration, 0.0f, EffectDurationMax); break;
	case Effect::Stun:        duration = FMath::Clamp(duration, 0.0f, EffectDurationMax); break;
	case Effect::Freeze:      duration = FMath::Clamp(duration, 0.0f, EffectDurationMax); break;
	}
	effectDuration[GetIndex(value)] = duration;
	return duration;
}
