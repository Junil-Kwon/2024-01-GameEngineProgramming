#include "Entity.h"
#include "Ghost.h"

#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"



bool AEntity::operator==(const AEntity& other) const { return this == &other; }

template<typename TEnum> FString AEntity::ToFString(TEnum value) {
	return StaticEnum<TEnum>()->GetNameStringByValue(static_cast<uint8>(value));
}
template<typename TEnum> TEnum AEntity::ToEnum(FString value) {
	int64 index = StaticEnum<TEnum>()->GetValueByName(FName(*value));
	return static_cast<TEnum>(index == INDEX_NONE ? 0 : index);
}
FRotator AEntity::ToRotator(FVector  value) {
	float degree = 90.0f - FMath::RadiansToDegrees(FMath::Atan2(value.X, value.Y));
	return FRotator(0.0f, degree, 0.0f);
}
FVector  AEntity::ToVector(FRotator value) {
	float radian = FMath::DegreesToRadians(value.Yaw);
	return FVector(FMath::Cos(radian), FMath::Sin(radian), 0.0f);
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
		FString name = StaticEnum<Identifier>()->GetNameStringByIndex(index);
		FString path = "/Game/Materials/" + name + "." + name;
		uTexture[index] = Cast<UTexture>(StaticLoadObject(UTexture::StaticClass(), nullptr, *path));
	}
	if (!uTexture[index]) UE_LOG(LogTemp, Warning, TEXT("Texture '%s' not found"), *ToFString(value));
	return uTexture[index];
}
UClass* AEntity::GetBlueprint(Identifier value) {
	static UClass* uClass[static_cast<uint8>(Identifier::Length)] = { nullptr, };
	uint8 index = static_cast<uint8>(value);
	if (uClass[index] == nullptr) {
		FString name = StaticEnum<Identifier>()->GetNameStringByIndex(index);
		FString path = "/Game/Blueprints/BP_" + name + ".BP_" + name + "_C";
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



void AEntity::CreateMaterial(UStaticMeshComponent* component) {
	if (component == nullptr) component = spriteComponent;
	UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(GetMaterialInstance(), this);
	material->SetTextureParameterValue(TEXT("Texture"), GetTexture(identifier));
	component->SetMaterial(0, material);
}
void AEntity::SetSpriteIndex(UStaticMeshComponent* component, int32 value) {
	if (component == nullptr) component = spriteComponent;
	component->SetScalarParameterValueOnMaterials(TEXT("Index"), value);
}
void AEntity::SetSpriteXFlip(UStaticMeshComponent* component, bool value) {
	if (component == nullptr) component = spriteComponent;
	component->SetScalarParameterValueOnMaterials(TEXT("XFlip"), value ? 1.0f : 0.0f);
}
void AEntity::SetSpriteColor(UStaticMeshComponent* component, FVector value) {
	if (component == nullptr) component = spriteComponent;
	component->SetVectorParameterValueOnMaterials(TEXT("Color"), value);
}
void AEntity::SetSpriteLight(UStaticMeshComponent* component, float value) {
	if (component == nullptr) component = spriteComponent;
	component->SetScalarParameterValueOnMaterials(TEXT("Intensity"), value);
}



AEntity* AEntity::SpawnEntity(Identifier value, FVector location) {
	return GetWorld()->SpawnActor<AEntity>(GetBlueprint(value), location, FRotator::ZeroRotator);
}



AEntity::AEntity() {
 	PrimaryActorTick.bCanEverTick = true;

	hitboxComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hitbox"));
	hitboxComponent->InitCapsuleSize(DefaultHitboxRadius, DefaultHitboxHeight * 0.5f);
	hitboxComponent->SetSimulatePhysics(true);
	hitboxComponent->SetCollisionProfileName(TEXT("Creature"));
	hitboxComponent->GetBodyInstance()->bLockXRotation = true;
	hitboxComponent->GetBodyInstance()->bLockYRotation = true;
	hitboxComponent->GetBodyInstance()->bLockZRotation = true;
	hitboxComponent->GetBodyInstance()->MassScale = mass;
	SetRootComponent(hitboxComponent);

	spriteComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sprite"));
	spriteComponent->SetWorldRotation(FRotator(0.0f, 90.0f, 41.409618f));
	spriteComponent->SetWorldScale3D(FVector(1.28f, 1.28f, 1.28f));
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
	zPrevious = GetActorLocation().Z;

	FString name = GetName();
	int64 BP = name.Find(TEXT("_"), ESearchCase::IgnoreCase, ESearchDir::FromStart,      0);
	int64 C0 = name.Find(TEXT("_"), ESearchCase::IgnoreCase, ESearchDir::FromStart, BP + 1);
	FString edit = name.Mid(BP + 1, C0 - BP - 1);
	identifier = ToEnum<Identifier>(edit);
	
	hitboxComponent->GetBodyInstance()->MassScale = mass;
	hitboxComponent->OnComponentHit.AddDynamic(this, &AEntity::OnHit);
	SetHitbox(hitboxRadius, hitboxHeight);

	CreateMaterial();

	shadowComponent->SetVisibility(true);
	shadowComponent->bCastHiddenShadow = true;
	shadowComponent->bHiddenInGame = true;

	SetGroup(group);

	uint8 tagTemp = tag;
	tag = 0;
	for (uint8 i = 0; i < static_cast<uint8>(Tag::Length); i++) {
		Tag value = static_cast<Tag>(1 << i);
		if (!(tagTemp & (1 << i))) continue;
		AddTag(value);
	}

	uint8 effectTemp = effect;
	effect = 0;
	effectStrength.Init(0.0f, static_cast<uint8>(Effect::Length));
	effectDuration.Init(0.0f, static_cast<uint8>(Effect::Length));
	float strength = 0.0f;
	float duration = 0.0f;
	for (uint8 i = 0; i < static_cast<uint8>(Effect::Length); i++) {
		Effect value = static_cast<Effect>(1 << i);
		if (!(effect & (1 << i)) || (effectImmunity & (1 << i))) continue;
		switch (value) {
		case Effect::HealthBoost: strength = 0.0f, duration = EffectMaxDuration; break;
		case Effect::DamageBoost: strength = 0.0f, duration = EffectMaxDuration; break;
		case Effect::Resistance:  strength = 0.0f, duration = EffectMaxDuration; break;
		case Effect::Speed:       strength = 0.0f, duration = EffectMaxDuration; break;
		case Effect::Burn:        strength = 0.0f, duration = EffectMaxDuration; break;
		case Effect::Stun:        strength = 0.0f, duration = EffectMaxDuration; break;
		case Effect::Freeze:      strength = 0.0f, duration = EffectMaxDuration; break;
		}
		AddEffect(value, strength, duration);
	}
}



AGhost* AEntity::GetGhost() {
	//static AGhost* aGhost = nullptr;
	//if (aGhost == nullptr) aGhost = Cast<AGhost>(UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0));
	//if (aGhost == nullptr) UE_LOG(LogTemp, Warning, TEXT("Ghost not found"));
	//return aGhost;
	return Cast<AGhost>(UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0));
}
float AEntity::GetWorldSpeed() {
	return GetGhost() ? GetGhost()->GetWorldSpeed() : DefaultWorldSpeed;
}
void  AEntity::SetWorldSpeed(float value) {
	if (!GetGhost()) return;
	GetGhost()->SetWorldSpeed(value);
	GetWorld()->GetWorldSettings()->WorldGravityZ = Gravity * value;
}
void  AEntity::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	float z = GetActorLocation().Z;
	if (z < zPrevious - 0.01f && !HasTag(Tag::Piercing)) {
		isFalling = true;
		fallSpeed = GetVelocity().Z;
	}
	else if (isFalling) {
		if (fallSpeed < ParticleThreshold && !HasTag(Tag::Piercing)) {
			SpawnEntity(Identifier::Dust, GetFootLocation() + FVector(0.0f, -hitboxRadius *  0.75f, 0.0f));
			SpawnEntity(Identifier::Dust, GetFootLocation() + FVector(0.0f, -hitboxRadius * -0.75f, 0.0f));
		}
		isFalling = false;
		fallSpeed = 0.0f;
	}
	if (z < VoidThreshold) {
		Destroy();
		return;
	}
	zPrevious = z;
	
	UpdateInputs(DeltaTime * GetWorldSpeed());
	UpdateAction(DeltaTime * GetWorldSpeed());
	UpdateEffect(DeltaTime * GetWorldSpeed());
	direction.Normalize();
	velocity = direction * GetWorldSpeed() * speed;
	if (HasEffect(Effect::Speed )) velocity *=        GetEffectStrength(GetIndex(Effect::Speed ));
	if (HasEffect(Effect::Freeze)) velocity *= 1.0f - GetEffectStrength(GetIndex(Effect::Freeze));
	
	// movement realization
	UE_LOG(LogTemp, Warning, TEXT("%f, %f"), velocity.X, velocity.Y);
	hitboxComponent->ComponentVelocity = velocity + (force *= 0.95f);
}
FVector AEntity::GetDirection() { return direction; }
void AEntity::SetDirection(FVector value) { direction = value; direction.Normalize(); }
void AEntity::AddForce(FVector value) { force += value / mass; }
bool AEntity::IsFalling() { return isFalling; }



Identifier AEntity::GetIdentifier() { return identifier; }



void AEntity::OnHitboxChanged() {
	hitboxComponent->SetCapsuleSize(hitboxRadius, hitboxHeight * 0.5f);
	if (hitboxRadius && hitboxHeight) {
		hitboxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RemoveTag(Tag::Piercing);
	}
	else {
		hitboxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		AddTag(Tag::Piercing);
	}
	FVector shadowScale = FVector(hitboxRadius * 2, hitboxRadius * 2, hitboxHeight) * 0.01f;
	shadowScale = shadowScale - FVector::OneVector * 0.2f;
	if (shadowScale.X <= 0.0f || shadowScale.Z <= 0.0f) shadowScale = FVector::ZeroVector;
	shadowComponent->SetWorldScale3D(shadowScale);
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



void AEntity::OnInteract(AEntity* other) {
}
void AEntity::OnHit(
	UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit) {
}



bool AEntity::GetInput(Action value) {
	if (!GetGhost()) return false;
	return GetGhost()->GetInput(value);
}
FVector AEntity::GetInputDirection() {
	if (!GetGhost()) return FVector::ZeroVector;
	return GetGhost()->GetInputDirection();
}



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
		SetAction(direction.IsZero() ? Action::Idle : Action::Move);
		for (uint8 i = 0; i < static_cast<uint8>(Action::Length); i++) {
			Action index = static_cast<Action>(1 << i);
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



Group AEntity::GetGroup() { return group; }
void AEntity::SetGroup(Group value) { group = value; }



bool AEntity::HasTag(Tag value) {
	return tag & static_cast<uint8>(value);
}
bool AEntity::AddTag(Tag value) {
	if (HasTag(value)) return false;
	tag |= static_cast<uint8>(value);
	switch (value) {
	case Tag::Floating:        hitboxComponent->GetBodyInstance()->bEnableGravity = false; break;
	case Tag::Piercing:        hitboxComponent->SetSimulatePhysics(false); break;
	case Tag::Invulnerability: break;
	case Tag::Interactability: break;
	case Tag::Collectable:     break;
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
	case Tag::Piercing:        hitboxComponent->SetSimulatePhysics(true); break;
	case Tag::Invulnerability: break;
	case Tag::Interactability: break;
	case Tag::Collectable:     break;
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
	switch (value) {
	case Effect::HealthBoost: break;
	case Effect::DamageBoost: break;
	case Effect::Resistance:  break;
	case Effect::Speed:       break;
	case Effect::Burn:        break;
	case Effect::Stun:        break;
	case Effect::Freeze:      break;
	}
	effect |= static_cast<uint8>(value);
	AdjustEffectStrength(index, strength);
	AdjustEffectDuration(index, duration);
	return true;
}
bool AEntity::RemoveEffect(Effect value) {
	if (!HasEffect(value)) return false;
	uint8 index = GetIndex(value);
	switch (value) {
	case Effect::HealthBoost: break;
	case Effect::DamageBoost: break;
	case Effect::Resistance:  break;
	case Effect::Speed:       break;
	case Effect::Burn:        break;
	case Effect::Stun:        break;
	case Effect::Freeze:      break;
	}
	effect &= ~static_cast<uint8>(value);
	effectStrength[index] = 0;
	effectDuration[index] = 0;
	return true;
}
uint8 AEntity::GetIndex(Effect value) {
	static uint8 list[static_cast<uint8>(Effect::Length)] = { 0, };
	uint8 index = static_cast<uint8>(value);
	if (list[index] == 0) for (uint8 i = 0; i < static_cast<uint8>(Effect::Length); i++) if (index & (1 << i)) {
		list[index] = i;
		break;
	}
	return list[index];
}
float AEntity::GetEffectStrength(uint8 index) { return effectStrength[index]; }
float AEntity::GetEffectDuration(uint8 index) { return effectDuration[index]; }
void  AEntity::AdjustEffectStrength(uint8 index, float strength) {
	Effect value = static_cast<Effect>(1 << index);
	strength = effectStrength[index] + strength;
	switch (value) {
	case Effect::HealthBoost: strength = FMath::Clamp(strength, 0.0f, EffectMaxStrength); break;
	case Effect::DamageBoost: strength = FMath::Clamp(strength, 0.0f, EffectMaxStrength); break;
	case Effect::Resistance:  strength = FMath::Clamp(strength, 0.0f, 1.0f);              break;
	case Effect::Speed:       strength = FMath::Clamp(strength, 0.0f, EffectMaxStrength); break;
	case Effect::Burn:        strength = FMath::Clamp(strength, 0.0f, EffectMaxStrength); break;
	case Effect::Stun:        strength = FMath::Clamp(strength, 0.0f, EffectMaxStrength); break;
	case Effect::Freeze:      strength = FMath::Clamp(strength, 0.0f, 1.0f);              break;
	}
	effectStrength[index] = strength;
	if (strength == 0.0f) RemoveEffect(value);
}
void  AEntity::AdjustEffectDuration(uint8 index, float duration) {
	Effect value = static_cast<Effect>(1 << index);
	duration = effectDuration[index] + duration;
	switch (value) {
	case Effect::HealthBoost: duration = FMath::Clamp(duration, 0.0f, EffectMaxDuration); break;
	case Effect::DamageBoost: duration = FMath::Clamp(duration, 0.0f, EffectMaxDuration); break;
	case Effect::Resistance:  duration = FMath::Clamp(duration, 0.0f, EffectMaxDuration); break;
	case Effect::Speed:       duration = FMath::Clamp(duration, 0.0f, EffectMaxDuration); break;
	case Effect::Burn:        duration = FMath::Clamp(duration, 0.0f, EffectMaxDuration); break;
	case Effect::Stun:        duration = FMath::Clamp(duration, 0.0f, EffectMaxDuration); break;
	case Effect::Freeze:      duration = FMath::Clamp(duration, 0.0f, EffectMaxDuration); break;
	}
	effectDuration[index] = duration;
	if (duration == 0.0f) RemoveEffect(value);
}
bool  AEntity::UpdateEffect(float DeltaTime) {
	for (uint8 i = 0; i < static_cast<uint8>(Effect::Length); i++) {
		Effect index = static_cast<Effect>(1 << i);
		if (HasEffect(index) && (effectDuration[i] -= DeltaTime) <= 0) RemoveEffect(index);
	}
	return true;
}
