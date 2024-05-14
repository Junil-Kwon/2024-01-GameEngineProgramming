#include "Entity.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"



FRotator VectorToRotator(FVector  value) {
	float degree = 90.0f - FMath::RadiansToDegrees(FMath::Atan2(value.X, value.Y));
	return FRotator(0.0f, degree, 0.0f);
}
FVector  RotatorToVector(FRotator value) {
	float radian = FMath::DegreesToRadians(value.Yaw);
	return FVector(FMath::Cos(radian), FMath::Sin(radian), 0.0f);
}



bool AEntity::operator==(const AEntity& other) const { return this == &other; }

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
UMaterialInstance* AEntity::GetMaterialInstance() {
	static UMaterialInstance* uInstance = nullptr;
	if (uInstance == nullptr) {
		FString path = "/Game/Materials/MaterialInstance.MaterialInstance";
		uInstance = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), nullptr, *path));
	}
	return uInstance;
}
UTexture* AEntity::GetTexture(Identifier value) {
	static UTexture* uTexture[static_cast<uint8>(Identifier::Max)] = { nullptr, };
	uint8 index = static_cast<uint8>(value);
	if (uTexture[index] == nullptr) {
		FString name = StaticEnum<Identifier>()->GetNameStringByIndex(index);
		FString path = "/Game/Materials/" + name + "." + name;
		uTexture[index] = Cast<UTexture>(StaticLoadObject(UTexture::StaticClass(), nullptr, *path));
	}
	return uTexture[index];
}
UClass* AEntity::GetBlueprint(Identifier value) {
	static UClass* uClass[static_cast<uint8>(Identifier::Max)] = { nullptr, };
	uint8 index = static_cast<uint8>(value);
	if (uClass[index] == nullptr) {
		FString name = StaticEnum<Identifier>()->GetNameStringByIndex(index);
		FString path = "/Game/Blueprints/BP_" + name + ".BP_" + name + "_C";
		uClass[index] = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *path));
	}
	return uClass[index];
}



AEntity::AEntity() {
 	PrimaryActorTick.bCanEverTick = true;
	GetPlaneMesh ();
	GetSphereMesh();
	GetMaterialInstance();

	hitboxComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hitbox"));
	hitboxComponent->InitCapsuleSize(DefaultHitboxRadius * 100, DefaultHitboxHeight * 50);
	hitboxComponent->SetSimulatePhysics(true);
	hitboxComponent->SetCollisionProfileName(TEXT("Creature"));
	SetRootComponent(hitboxComponent);

	spriteComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sprite"));
	spriteComponent->SetWorldRotation(FRotator(0.0f, 90.0f, 41.409618f));
	spriteComponent->SetWorldScale3D(FVector(1.28f, 1.28f, 1.28f));
	spriteComponent->SetStaticMesh(GetPlaneMesh());
	spriteComponent->SetMaterial(0, GetMaterialInstance());
	spriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	spriteComponent->SetupAttachment(hitboxComponent);

	shadowComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shadow"));
	shadowComponent->SetStaticMesh(GetSphereMesh());
	shadowComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	shadowComponent->SetVisibility(false);
	shadowComponent->SetupAttachment(hitboxComponent);
}

void AEntity::BeginPlay() {
	Super::BeginPlay();

	zPrev = GetActorLocation().Z;

	FString name = GetName();
	FString edit;
	for (int64 i = 3; i < name.Len(); i++) {
		if (FChar::IsDigit(name[i])) continue;
		if (name[i] == '_') continue;
		if (name[i] == ' ') continue;
		edit.AppendChar(name[i]);
	}
	int64 index = FindObject<UEnum>(ANY_PACKAGE, TEXT("Identifier"), true)->GetIndexByName(*edit);
	if (index != INDEX_NONE) identifier = static_cast<Identifier>(index);

	SetHitbox(hitboxRadius, hitboxHeight);
	hitboxComponent->GetBodyInstance()->bLockXRotation = true;
	hitboxComponent->GetBodyInstance()->bLockYRotation = true;
	hitboxComponent->GetBodyInstance()->bLockZRotation = true;
	hitboxComponent->GetBodyInstance()->MassScale = mass;
	
	if (sprite == Identifier::Default) sprite = identifier;
	material = UMaterialInstanceDynamic::Create(GetMaterialInstance(), this);
	spriteComponent->SetMaterial(0, material);
	SetSprite(sprite);

	shadowComponent->SetVisibility(true);
	shadowComponent->bCastHiddenShadow = true;
	shadowComponent->bHiddenInGame = true;

	SetGroup(group);

	uint8 tagTemp = tag;
	tag = 0;
	for (uint8 i = 0; i < static_cast<uint8>(Tag::Max); i++) {
		Tag value = static_cast<Tag>(1 << i);
		if ((tagTemp & (1 << i)) == 0) continue;
		AddTag(value);
	}

	uint8 effectTemp = effect;
	effect = 0;
	effectStrength.Init(0.0f, static_cast<uint8>(Effect::Max));
	effectDuration.Init(0.0f, static_cast<uint8>(Effect::Max));
	float strength = 0.0f;
	float duration = 0.0f;
	for (uint8 i = 0; i < static_cast<uint8>(Effect::Max); i++) {
		Effect value = static_cast<Effect>(1 << i);
		if ((effect & (1 << i)) == 0 || (effectImmunity & (1 << i)) != 0) continue;
		switch (value) {
		case Effect::HealthBoost: strength = 0.0f, duration = EffectMaxDuration; break;
		case Effect::DamageBoost: strength = 0.0f, duration = EffectMaxDuration; break;
		case Effect::Resistance:  strength = 0.0f, duration = EffectMaxDuration; break;
		case Effect::Speed:       strength = 0.0f, duration = EffectMaxDuration; break;
		case Effect::Burn:        strength = 0.0f, duration = EffectMaxDuration; break;
		case Effect::Stun:        strength = 0.0f, duration = EffectMaxDuration; break;
		case Effect::Freeze:      strength = 0.0f, duration = EffectMaxDuration; break;
		case Effect::Slowness:    strength = 0.0f, duration = EffectMaxDuration; break;
		}
		AddEffect(value, strength, duration);
	}
}



void AEntity::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	float z = GetActorLocation().Z;
	if (z < zPrev - 0.01f) {
		isFalling = true;
		fallSpeed = -GetVelocity().Z;
	}
	else if (isFalling) {
		if (ParticleThreshold < fallSpeed) {
			Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, -hitboxRadius *  0.75f, 0.0f));
			Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, -hitboxRadius * -0.75f, 0.0f));
		}
		UE_LOG(LogTemp, Warning, TEXT("%f"), fallSpeed);
		isFalling = false;
		fallSpeed = 0.0f;
	}
	zPrev = z;

	UpdateSprite(DeltaTime);
	UpdateAction(DeltaTime);
	UpdateEffect(DeltaTime);
}
bool AEntity::IsFalling() { return isFalling; }



Identifier AEntity::GetIdentifier() {
	return identifier;
}
AEntity* AEntity::Spawn(Identifier value, FVector location) {
	if (GetBlueprint(value) == nullptr) return nullptr;
	return GetWorld()->SpawnActor<AEntity>(GetBlueprint(value), location, FRotator::ZeroRotator);
}



void AEntity::OnHitboxChanged() {
	hitboxComponent->SetCapsuleSize(hitboxRadius * 100, hitboxHeight * 50);
	if (hitboxRadius && hitboxHeight) {
		hitboxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RemoveTag(Tag::Piercing);
	}
	else {
		hitboxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		AddTag(Tag::Piercing);
	}
	FVector scale = FVector(hitboxRadius * 2, hitboxRadius * 2, hitboxHeight);
	shadowComponent->SetWorldScale3D(scale - FVector::OneVector * 0.2f);
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



int32 AEntity::GetSpriteIndex() { return spriteIndex; }
bool  AEntity::GetSpriteXFlip() { return spriteXFlip; }
void  AEntity::SetSpriteIndex(int32 value) {
	spriteIndex = value;
	spriteComponent->SetScalarParameterValueOnMaterials(TEXT("Index"), value);
}
void  AEntity::SetSpriteXFlip(bool value) {
	spriteXFlip = value;
	spriteComponent->SetScalarParameterValueOnMaterials(TEXT("XFlip"), value ? 1.0f : 0.0f);
}
void  AEntity::UpdateSprite(float DeltaTime) {
	spriteDelay += DeltaTime;
}
Identifier AEntity::GetSprite() {
	return sprite;
}
void AEntity::SetSprite(Identifier value) {
	if (GetTexture(value) == nullptr) return;
	material->SetTextureParameterValue(TEXT("Texture"), GetTexture(value));
}



Action AEntity::GetAction() {
	return action;
}
void AEntity::SetAction(Action value) {
	action = value;
	actionDelay = 0.0f;
	switch (value) {
	case Action::Idle:   break;
	case Action::Move:   break;
	case Action::Jump:   break;
	case Action::Dodge:  break;
	case Action::Defeat: break;
	}
}
void AEntity::UpdateAction(float DeltaTime) {
	actionDelay += DeltaTime;
	switch (action) {
	case Action::Idle:   break;
	case Action::Move:   break;
	case Action::Jump:   break;
	case Action::Dodge:  break;
	case Action::Defeat: break;
	}
}



Group AEntity::GetGroup() {
	return group;
}
void AEntity::SetGroup(Group value) {
	group = value;
}



bool AEntity::HasTag(Tag value) {
	return tag & static_cast<uint8>(value);
}
bool AEntity::AddTag(Tag value) {
	if (HasTag(value)) return false;
	tag |= static_cast<uint8>(value);
	switch (value) {
	case Tag::Floating:        hitboxComponent->GetBodyInstance()->bEnableGravity = false; break;
	case Tag::Piercing:        hitboxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); break;
	case Tag::Invulnerability: break;
	case Tag::Interactability: break;
	case Tag::Player:          break;
	case Tag::Leader:          break;
	}
	return true;
}
bool AEntity::RemoveTag(Tag value) {
	if (!HasTag(value)) return false;
	tag &= ~static_cast<uint8>(value);
	switch (value) {
	case Tag::Floating:        hitboxComponent->GetBodyInstance()->bEnableGravity = true; break;
	case Tag::Piercing:        hitboxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); break;
	case Tag::Invulnerability: break;
	case Tag::Interactability: break;
	case Tag::Player:          break;
	case Tag::Leader:          break;
	}
	return true;
}



bool AEntity::HasEffect(Effect value) {
	return static_cast<uint8>(effect) & static_cast<uint8>(value);
}
bool AEntity::AddEffect(Effect value, float strength, float duration) {
	if (static_cast<uint8>(effectImmunity) & static_cast<uint8>(value)) return false;
	uint8 index = GetIndex(value);
	effect |= static_cast<uint8>(value);
	effectStrength[index] = strength;
	effectDuration[index] = duration;
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
	uint8 index = GetIndex(value);
	effect &= ~static_cast<uint8>(value);
	effectStrength[index] = 0;
	effectDuration[index] = 0;
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
	static uint8 list[static_cast<uint8>(Effect::Max)] = { 0, };
	uint8 index = static_cast<uint8>(value);
	if (list[index] == 0) for (uint8 i = 0; i < static_cast<uint8>(Effect::Max); i++) if (index & (1 << i)) {
		list[index] = i;
		break;
	}
	return list[index];
}
float AEntity::GetEffectStrength(uint8 value) { return effectStrength[value]; }
float AEntity::GetEffectDuration(uint8 value) { return effectDuration[value]; }
void  AEntity::SetEffectStrength(uint8 value, float strength) { effectStrength[value] = strength; }
void  AEntity::SetEffectDuration(uint8 value, float duration) { effectDuration[value] = duration; }
void  AEntity::UpdateEffect(float DeltaTime) {
	for (uint8 i = 0; i < static_cast<uint8>(Effect::Max); i++) {
		Effect index = static_cast<Effect>(1 << i);
		if (HasEffect(index) && (effectDuration[i] -= DeltaTime) <= 0) RemoveEffect(index);
	}
}
