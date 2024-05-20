#include "Entity.h"
#include "Ghost.h"

#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"

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



uint8 AEntity::GetIndex(Tag value) {
	switch (value) {
	case Tag::Floating:        return 1 << 0;
	case Tag::Piercing:        return 1 << 1;
	case Tag::Invulnerability: return 1 << 2;
	case Tag::Interactability: return 1 << 3;
	case Tag::Collectable:     return 1 << 4;
	case Tag::Player:          return 1 << 5;
	case Tag::Leader:          return 1 << 6;
	}
	return 0;
}
uint8 AEntity::GetIndex(Effect value) {
	switch (value) {
	case Effect::HealthBoost: return 1 << 0;
	case Effect::DamageBoost: return 1 << 1;
	case Effect::Resistance:  return 1 << 2;
	case Effect::Speed:       return 1 << 3;
	case Effect::Burn:        return 1 << 4;
	case Effect::Stun:        return 1 << 5;
	case Effect::Freeze:      return 1 << 6;
	}
	return 0;
}
template<typename TEnum> FString AEntity::ToFString(TEnum value) {
	return StaticEnum<TEnum>()->GetNameStringByValue(static_cast<uint8>(value));
}
template<typename TEnum> TEnum AEntity::ToEnum(FString value) {
	int64 index = StaticEnum<TEnum>()->GetValueByName(FName(*value));
	return static_cast<TEnum>(index == INDEX_NONE ? 0 : index);
}



UStaticMesh* AEntity::GetPlaneMesh() {
	static UStaticMesh* uStaticMesh = nullptr;
	if (uStaticMesh == nullptr) {
		FString path = "/Engine/BasicShapes/Plane.Plane";
		uStaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *path));
	}
	return uStaticMesh;
}
UStaticMesh* AEntity::GetSphereMesh() {
	static UStaticMesh* uStaticMesh = nullptr;
	if (uStaticMesh == nullptr) {
		FString path = "/Engine/BasicShapes/Sphere.Sphere";
		uStaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *path));
	}
	return uStaticMesh;
}
UTexture* AEntity::GetTexture(Identifier value) {
	static UTexture* uTexture[static_cast<uint8>(Identifier::Length)] = { nullptr, };
	uint8 index = static_cast<uint8>(value);
	if (uTexture[index] == nullptr) {
		FString path = "/Game/Materials/" + ToFString(value) + "." + ToFString(value);
		uTexture[index] = Cast<UTexture>(StaticLoadObject(UTexture::StaticClass(), nullptr, *path));
	}
	if (!uTexture[index]) UE_LOG(LogTemp, Warning, TEXT("Texture '%s' not found"), *ToFString(value));
	return uTexture[index];
}
UClass* AEntity::GetBlueprint(Identifier value) {
	static UClass* uClass[static_cast<uint8>(Identifier::Length)] = { nullptr, };
	uint8 index = static_cast<uint8>(value);
	if (uClass[index] == nullptr) {
		FString path = "/Game/Blueprints/BP_" + ToFString(value) + ".BP_" + ToFString(value) + "_C";
		uClass[index] = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *path));
	}
	if (!uClass[index]) UE_LOG(LogTemp, Warning, TEXT("Blueprint 'BP_%s' not found"), *ToFString(value));
	return uClass[index];
}
UMaterialInstance* AEntity::GetMaterialInstance() {
	static UMaterialInstance* uInstance = nullptr;
	if (uInstance == nullptr) {
		FString path = "/Game/Materials/MaterialInstance.MaterialInstance";
		uInstance = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), nullptr, *path));
	}
	if (!uInstance) UE_LOG(LogTemp, Warning, TEXT("Material 'MaterialInstance' not found"));
	return uInstance;
}



AEntity* AEntity::Spawn(Identifier value, FVector location) {
	UClass* uClass = GetBlueprint(value);
	if (!uClass) return nullptr;
	static FActorSpawnParameters parameter;
	if (parameter.Instigator == nullptr) {
		parameter.Instigator = this;
		parameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	}
	return GetWorld()->SpawnActor<AEntity>(GetBlueprint(value), location, FRotator::ZeroRotator, parameter);
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

	uArrowComponent = GetArrowComponent();

	spriteComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sprite"));
	spriteComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 41.409618f));
	spriteComponent->SetRelativeScale3D(FVector(1.28f, 1.28f, 1.28f));
	spriteComponent->SetStaticMesh(GetPlaneMesh());
	spriteComponent->SetMaterial(0, GetMaterialInstance());
	spriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	spriteComponent->SetupAttachment(RootComponent);

	shadowComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shadow"));
	shadowComponent->SetStaticMesh(GetSphereMesh());
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
	identifier = ToEnum<Identifier>(edit);

	SetHitbox(defaultHitboxRadius, defaultHitboxHeight);
	CreateMaterial();

	shadowComponent->SetVisibility(true);
	shadowComponent->bCastHiddenShadow = true;
	shadowComponent->bHiddenInGame = true;

	speed = defaultSpeed;
	GetCharacterMovement()->JumpZVelocity = 800.0f;
	GetCharacterMovement()->GravityScale = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = speed;
	GetCharacterMovement()->MaxAcceleration = speed * 100;

	SetGroup(defaultGroup);
	for (uint8 i = 0; i < static_cast<uint8>(Tag::Length); i++) {
		if (defaultTag & (1 << i)) AddTag(static_cast<Tag>(1 << i));
	}
	effectImmunity = defaultEffectImmunity;
	for (uint8 i = 0; i < static_cast<uint8>(Effect::Length); i++) {
		if (defaultEffect & (1 << i)) AddEffect(static_cast<Effect>(1 << i));
	}
}

// =============================================================================================================
// Update
// =============================================================================================================

void AEntity::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsFalling()) {
		if (!isFalling) isFalling = true;
		fallSpeed = GetCharacterMovement()->Velocity.Z;
		if (fallSpeed < MaxFallSpeed) GetCharacterMovement()->Velocity.Z = MaxFallSpeed;
		if (GetFootLocation().Z < VoidThreshold) Destroy();
	}
	else if (isFalling) {
		if (fallSpeed < DustThreshold) {
			Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, -hitboxRadius *  0.75f, 0.0f));
			Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, -hitboxRadius * -0.75f, 0.0f));
		}
		isFalling = false;
		fallSpeed = 0.0f;
	}

	UpdateInputs(DeltaTime);
	UpdateAction(DeltaTime);
	UpdateEffect(DeltaTime);
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
	return GetActorLocation() + FVector(-hitboxHeight * 0.5f, 0.0f, -hitboxHeight * 0.5f + 48.0f);
}

// =============================================================================================================
// Arrow
// =============================================================================================================

FVector AEntity::GetArrowDirection() { return uArrowComponent->GetForwardVector(); }
void    AEntity::SetArrowDirection(FVector value) { uArrowComponent->SetWorldRotation(ToRotator(value)); }

// =============================================================================================================
// Sprite
// =============================================================================================================

void AEntity::CreateMaterial(UStaticMeshComponent* component) {
	if (component == nullptr) component = spriteComponent;
	UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(GetMaterialInstance(), this);
	material->SetTextureParameterValue(TEXT("Texture"), GetTexture(identifier));
	component->SetMaterial(0, material);
}

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
	if (!VerifyAction(value)) return false;
	action = value;
	actionDelay = 0.0f;
	return true;
}
bool AEntity::VerifyAction(Action value) {
	return false;
}
bool AEntity::UpdateInputs(float DeltaTime) {
	if (HasTag(Tag::Player)) {
		direction = GetInputDirection();
		if (action == Action::Idle && !direction.IsZero()) SetAction(Action::Move);
		if (action == Action::Move &&  direction.IsZero()) SetAction(Action::Idle);
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
	switch (action) {
	case Action::Idle:   break;
	case Action::Move:   break;
	case Action::Jump:   break;
	case Action::Dash:   break;
	case Action::Attack: break;
	case Action::Defend: break;
	case Action::Defeat: break;
	}
	return true;
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
	case Tag::Piercing:        hitboxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); break;
	case Tag::Invulnerability: break;
	case Tag::Interactability: break;
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
	case Tag::Floating:        GetCharacterMovement()->GravityScale = 3.0f; break;
	case Tag::Piercing:        hitboxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); break;
	case Tag::Invulnerability: break;
	case Tag::Interactability: break;
	case Tag::Collectable:     break;
	case Tag::Player:          break;
	case Tag::Leader:          break;
	}
	return true;
}

// =============================================================================================================
// Effect
// =============================================================================================================

bool AEntity::UpdateEffect(float DeltaTime) {
	for (uint8 i = 0; i < static_cast<uint8>(Effect::Length); i++) {
		Effect index = static_cast<Effect>(1 << i);
		if (HasEffect(index) && (effectDuration[i] -= DeltaTime) <= 0) RemoveEffect(index);
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
	effectStrength[GetIndex(value)] = 0;
	effectDuration[GetIndex(value)] = 0;
	return true;
}

float AEntity::GetEffectStrength(Effect value) { return effectStrength[GetIndex(value)]; }
float AEntity::GetEffectDuration(Effect value) { return effectDuration[GetIndex(value)]; }
void  AEntity::AdjustEffectStrength(Effect value, float strength) {
	strength = effectStrength[GetIndex(value)] + strength;
	switch (value) {
	case Effect::HealthBoost: strength = FMath::Clamp(strength, 0.0f, EffectMaxStrength); break;
	case Effect::DamageBoost: strength = FMath::Clamp(strength, 0.0f, EffectMaxStrength); break;
	case Effect::Resistance:  strength = FMath::Clamp(strength, 0.0f, 1.0f);              break;
	case Effect::Speed:       strength = FMath::Clamp(strength, 0.0f, EffectMaxStrength); break;
	case Effect::Burn:        strength = FMath::Clamp(strength, 0.0f, EffectMaxStrength); break;
	case Effect::Stun:        strength = FMath::Clamp(strength, 0.0f, EffectMaxStrength); break;
	case Effect::Freeze:      strength = FMath::Clamp(strength, 0.0f, 1.0f);              break;
	}
	effectStrength[GetIndex(value)] = strength;
	if (strength == 0.0f) RemoveEffect(value);
}
void  AEntity::AdjustEffectDuration(Effect value, float duration) {
	duration = effectDuration[GetIndex(value)] + duration;
	switch (value) {
	case Effect::HealthBoost: duration = FMath::Clamp(duration, 0.0f, EffectMaxDuration); break;
	case Effect::DamageBoost: duration = FMath::Clamp(duration, 0.0f, EffectMaxDuration); break;
	case Effect::Resistance:  duration = FMath::Clamp(duration, 0.0f, EffectMaxDuration); break;
	case Effect::Speed:       duration = FMath::Clamp(duration, 0.0f, EffectMaxDuration); break;
	case Effect::Burn:        duration = FMath::Clamp(duration, 0.0f, EffectMaxDuration); break;
	case Effect::Stun:        duration = FMath::Clamp(duration, 0.0f, EffectMaxDuration); break;
	case Effect::Freeze:      duration = FMath::Clamp(duration, 0.0f, EffectMaxDuration); break;
	}
	effectDuration[GetIndex(value)] = duration;
	if (duration == 0.0f) RemoveEffect(value);
}
