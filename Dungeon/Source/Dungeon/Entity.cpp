#include "Entity.h"
#include "Ghost.h"

#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Engine/Font.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"





// =============================================================================================================
// Default
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
	return FRotator(-48.590382f, 0.0f, 0.0f).RotateVector(value);
}

template<typename TEnum> uint8 AEntity::GetIndex(TEnum value) {
	static uint8 list[255] = { 0, };
	uint8 index = static_cast<uint8>(value);
	if (!std::is_same_v<TEnum, Tag> && !std::is_same_v<TEnum, Effect>) return index;
	if (!list[1]) for (uint8 i = 0; i < 8; i++) list[uint8(1 << i)] = i;
	return list[index];
}
template<typename TEnum> FString AEntity::ToString(TEnum value) {
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
		FString name = ToString(value);
		FString path = "/Engine/BasicShapes/" + name + "." + name;
		uStaticMesh[index] = LoadObject<UStaticMesh>(nullptr, *path);
	}
	return uStaticMesh[index];
}
UTexture2D* AEntity::GetTexture2D(Identifier value) {
	static UTexture2D* uTexture2D[static_cast<uint8>(Identifier::Length)] = { nullptr, };
	uint8 index = static_cast<uint8>(value);
	if (uTexture2D[index] == nullptr) {
		FString name = ToString(value);
		FString path = "/Game/Textures/" + name + "." + name;
		uTexture2D[index] = LoadObject<UTexture2D>(nullptr, *path);
	}
	return uTexture2D[index];
}
UMaterialInstanceDynamic* AEntity::GetMaterialInstanceDynamic(Identifier value) {
	static UMaterialInstance* uInstance = nullptr;
	if (uInstance == nullptr) {
		FString path = "/Game/Textures/MaterialInstance.MaterialInstance";
		uInstance = LoadObject<UMaterialInstance>(nullptr, *path);
	}
	UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(uInstance, this);
	material->SetTextureParameterValue(TEXT("Texture"), GetTexture2D(value));
	return material;
}

UFont* AEntity::GetFont(FontType value) {
	static UFont* uFont[static_cast<uint8>(FontType::Length)] = { nullptr, };
	uint8 index = static_cast<uint8>(value);
	if (uFont[index] == nullptr) {
		FString name = ToString(value);
		FString path = "/Game/Fonts/" + name + "-Font." + name + "-Font";
		uFont[index] = LoadObject<UFont>(nullptr, *path);
	}
	return uFont[index];
}
UMaterial* AEntity::GetFontMaterial(FontType value) {
	static UMaterial* uMaterial[static_cast<uint8>(FontType::Length)] = { nullptr, };
	uint8 index = static_cast<uint8>(value);
	if (uMaterial[index] == nullptr) {
		FString name = ToString(value);
		FString path = "/Game/Fonts/" + name + "-Material." + name + "-Material";
		uMaterial[index] = LoadObject<UMaterial>(nullptr, *path);
	}
	return uMaterial[index];
}

AEntity* AEntity::Spawn(Identifier value, FVector location) {
	AEntity* entity = nullptr;
	if (GetGhost()->GetObjectPool(value)->Num() == 0) {
		static UClass* uClass[static_cast<uint8>(Identifier::Length)] = { nullptr, };
		uint8 index = static_cast<uint8>(value);
		if (uClass[index] == nullptr) {
			FString name = ToString(value);
			FString path = "/Game/Blueprints/BP_" + name + ".BP_" + name + "_C";
			uClass[index] = LoadObject<UClass>(nullptr, *path);
		}
		if (uClass[index] == nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("%s Spawn Failed."), *ToString(value));
			return nullptr;
		}
		FActorSpawnParameters parameter;
		parameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		entity = GetWorld()->SpawnActor<AEntity>(uClass[index], location, FRotator::ZeroRotator, parameter);
	}
	else {
		entity = GetGhost()->GetObjectPool(value)->Pop();
		entity->SetActorLocation(location);
		entity->SetActorTickEnabled(true);
		entity->SetActorHiddenInGame(false);
		entity->hitboxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		entity->GetCharacterMovement()->StopMovementImmediately();
		entity->active = true;
		entity->OnSpawn();
	}
	return entity;
}
void AEntity::Despawn() {
	OnDespawn();
	Destroy();
	//Remove AI Controller
	/*
	GetGhost()->GetObjectPool(GetIdentifier())->Push(this);
	SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	hitboxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->GravityScale = 0;
	if (IsAttached()) Detach();
	active = false;
	OnDespawn();
	*/
}

bool AEntity::IsAttached() {
	return GetAttachParentActor() != nullptr;
}
void AEntity::Attach(AEntity* entity) {
	if (entity == nullptr) return;
	GetCharacterMovement()->StopMovementImmediately();
	AttachToActor(entity, FAttachmentTransformRules::KeepWorldTransform);
}
void AEntity::Detach() {
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

// =============================================================================================================
// Ghost
// =============================================================================================================

AGhost* AEntity::GetGhost() {
	if (ghost == nullptr) ghost = Cast<AGhost>(UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0));
	return ghost;
}





// =============================================================================================================
// Initialization
// =============================================================================================================

AEntity::AEntity() {
	defaultSpeed          = 300.0f;

	defaultHitboxRadius   = 0.0f;
	defaultHitboxHeight   = 0.0f;
	defaultHandLocation   = FVector2D::ZeroVector;

	defaultGroup          = Group::None;
	defaultEffect         = 0;
	defaultEffectImmunity = 0;

	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	hitboxComponent = GetCapsuleComponent();
	hitboxComponent->InitCapsuleSize(defaultHitboxRadius, defaultHitboxHeight * 0.5f);
	hitboxComponent->SetCollisionProfileName(TEXT("Entity"));
	hitboxComponent->SetNotifyRigidBodyCollision(true);
	SetRootComponent(hitboxComponent);

	anchorComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Anchor"));
	anchorComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 41.409618f));
	anchorComponent->SetupAttachment(RootComponent);

	spriteComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sprite"));
	spriteComponent->SetStaticMesh(GetStaticMesh(MeshType::Plane));
	spriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	spriteComponent->SetupAttachment(anchorComponent);

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

	hitboxComponent->OnComponentHit.AddDynamic(this, &AEntity::OnHit);
	hitboxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEntity::OnHitboxBeginOverlap);

	FVector scale = FVector::OneVector * GetTexture2D(GetIdentifier())->GetSizeX() * 0.005f;
	spriteComponent->SetMaterial(0, GetMaterialInstanceDynamic(GetIdentifier()));
	spriteComponent->SetRelativeScale3D(scale);

	shadowComponent->SetVisibility(true);
	shadowComponent->bCastHiddenShadow = true;
	shadowComponent->bHiddenInGame = true;

	active = true;
	OnSpawn();
}

// =============================================================================================================
// Spawn
// =============================================================================================================

bool AEntity::IsActive() {
	return active;
}
void AEntity::OnSpawn() {
	speed = defaultSpeed;
	isFalling = false;
	fallSpeed = 0.0f;
	GetCharacterMovement()->MaxWalkSpeed    = speed;
	GetCharacterMovement()->MaxAcceleration = speed * 100;
	GetCharacterMovement()->JumpZVelocity   = 800.0f;
	GetCharacterMovement()->GravityScale    = DefaultGravityScale;

	SetLookDirection(FVector(0.0f, 1.0f, 0.0f));
	SetMoveDirection(FVector(0.0f, 0.0f, 0.0f));

	SetHitbox(defaultHitboxRadius, defaultHitboxHeight);
	handLocation = FVector(0.0f, defaultHandLocation.X, defaultHandLocation.Y);

	SetSpriteIndex    (nullptr);
	SetSpriteXFlip    (nullptr);
	SetSpriteColor    (nullptr);
	SetSpriteIntensity(nullptr);
	SetSpriteOpacity  (nullptr);
	SetSpriteAngle    (nullptr);

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
void AEntity::OnDespawn() {
	if (HasTag(Tag::Player)) GetGhost()->SetPlayer(nullptr);
	for (uint8 i = 0; i < static_cast<uint8>(Tag::Length); i++) {
		Tag value = static_cast<Tag>(1 << i);
		if (HasTag(value)) RemoveTag(value);
	}
	for (uint8 i = 0; i < static_cast<uint8>(Effect::Length); i++) {
		Effect value = static_cast<Effect>(1 << i);
		if (HasEffect(value)) RemoveEffect(value);
	}
}

// =============================================================================================================
// Update
// =============================================================================================================

void AEntity::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (active) Update(DeltaTime);
}

void AEntity::Update(float DeltaTime) {
	lifeTime += DeltaTime;
	if (GetCharacterMovement()->IsFalling()) {
		if (!isFalling) isFalling = true;
		fallSpeed = GetCharacterMovement()->Velocity.Z;
		if (fallSpeed < FallSpeedMax) GetCharacterMovement()->Velocity.Z = FallSpeedMax;
		if (GetFootLocation().Z < VoidZAxis) {
			if (GetGhost()->GetPlayer() && GetGroup() == GetGhost()->GetPlayer()->GetGroup()) {
				SetActorLocation(lastLocation);
				GetCharacterMovement()->StopMovementImmediately();
				Damage(1.0f);
			}
			else Despawn();
		}
	}
	else {
		lastLocation = GetActorLocation();
		if (isFalling) {
			if (fallSpeed < DustThreshold) {
				Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, -hitboxRadius *  0.75f, 0.0f));
				Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, -hitboxRadius * -0.75f, 0.0f));
			}
			isFalling = false;
			fallSpeed = 0.0f;
		}
	}
	UpdateEffect(DeltaTime);
}
float AEntity::GetLifeTime() { return lifeTime; }

FVector AEntity::GetLookDirection() { return lookDirection; }
FVector AEntity::GetMoveDirection() { return moveDirection; }
void AEntity::SetLookDirection(FVector value) {
	if (value.IsZero()) return;
	value.Normalize();
	lookDirection = value;
	if (lookDirection.Y != 0 && GetSpriteXFlip() != (lookDirection.Y < 0.0f)) {
		SetSpriteXFlip(nullptr, lookDirection.Y < 0.0f);
	}
}
void AEntity::SetMoveDirection(FVector value) {
	value.Normalize();
	moveDirection = value;
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
FVector AEntity::GetHandLocation() {
	FVector hand = FVector(-4.0f, handLocation.Y * (!GetSpriteXFlip() ? 1.0f : -1.0f), handLocation.Z);
	return GetActorLocation() + RotateVector(hand);
}
FVector AEntity::GetFootLocation() {
	return GetActorLocation() + RotateVector(FVector(-4.0f, 0.0f, -hitboxHeight * 0.5f + 8.0f));
}

void AEntity::OnHit(
	UPrimitiveComponent* HitComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse,
	const FHitResult& Hit) {
	AEntity* entity = static_cast<AEntity*>(OtherActor);
	if (OtherActor && OtherActor->IsA(AEntity::StaticClass())) OnCollision(entity);
}
void AEntity::OnHitboxBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult) {
	AEntity* entity = static_cast<AEntity*>(OtherActor);
	if (OtherActor && OtherActor->IsA(AEntity::StaticClass())) OnCollision(entity);
}
void AEntity::OnCollision(AEntity* entity) {
}
void AEntity::OnInteract (AEntity* entity) {
}

// =============================================================================================================
// Sprite
// =============================================================================================================

USceneComponent* AEntity::GetAnchorComponent() { return anchorComponent; }

int32   AEntity::GetSpriteIndex    () { return spriteIndex;     }
bool    AEntity::GetSpriteXFlip    () { return spriteXFlip;     }
FVector AEntity::GetSpriteColor    () { return spriteColor;     }
float   AEntity::GetSpriteIntensity() { return spriteIntensity; }
float   AEntity::GetSpriteOpacity  () { return spriteOpacity;   }
float   AEntity::GetSpriteAngle    () { return spriteAngle;     }

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
void AEntity::SetSpriteOpacity(UStaticMeshComponent* component, float value) {
	if (component == nullptr) {
		if (value == spriteOpacity) return;
		component = spriteComponent;
		spriteOpacity = value;
	}
	component->SetScalarParameterValueOnMaterials(TEXT("Opacity"), value);
}
void AEntity::SetSpriteAngle(UStaticMeshComponent* component, float value) {
	if (component == nullptr) {
		if (value == spriteAngle) return;
		component = spriteComponent;
		spriteAngle = value;
	}
	component->SetRelativeRotation(FRotator(0.0f, value, 0.0f));
}





// =============================================================================================================
// Identifier
// =============================================================================================================

Identifier AEntity::GetIdentifier() { return identifier; }

// =============================================================================================================
// Group
// =============================================================================================================

Group AEntity::GetGroup() { return group; }
void  AEntity::SetGroup(Group value) { group = value; }

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
	case Tag::Floating:        GetCharacterMovement()->GravityScale = DefaultGravityScale; break;
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

void AEntity::Damage(float value) {
	effectHit = EffectHitDuration;
}
void AEntity::UpdateEffect(float DeltaTime) {
	for (uint8 i = 0; i < static_cast<uint8>(Effect::Length); i++) {
		Effect value = static_cast<Effect>(1 << i);
		if (!HasEffect(value)) continue;
		float duration = AdjustEffectDuration(value, -DeltaTime);
		switch (value) {
		case Effect::Burn:
			if (int32(duration * 4) != int32((duration - DeltaTime) * 4)) {
				FVector location = FVector::ZeroVector;
				float angle = FMath::RandRange(-0.2f * PI, 1.2f * PI);
				location.X = -4.0f;
				location.Y = hitboxRadius * FMath::Cos(angle) * FMath::RandRange(0.6f, 0.8f);
				location.Z = hitboxHeight * FMath::Sin(angle) * FMath::RandRange(0.3f, 0.4f);
				Spawn(Identifier::Flame, GetActorLocation() + RotateVector(location))->Attach(this);
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
			case Effect::Slowness:
			case Effect::Stun:
			default:
				RemoveEffect(value);
				break;
			}
		}
	}
	if (refreshColor || effectHit) {
		refreshColor = false;
		effectHit = FMath::Max(effectHit - DeltaTime, 0.0f);
		FVector color     = GetSpriteColor    ();
		float   intensity = GetSpriteIntensity() + effectHit * 7.5f;

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
		color = (color - GetSpriteColor() != FVector::ZeroVector) ? color / color.GetMax() : color;
		SetSpriteColor    (spriteComponent, color    );
		SetSpriteIntensity(spriteComponent, intensity);
	}
	if (refreshSpeed) {
		float i = 1.0f;
		if (HasEffect(Effect::Speed )) i *= 1.0f + GetEffectStrength(Effect::Speed);
		if (HasEffect(Effect::Burn  )) i *= 1.0f + FMath::Max(GetEffectStrength(Effect::Burn) * 0.3f, 0.3f);
		if (HasEffect(Effect::Freeze)) i *= 1.0f - GetEffectStrength(Effect::Freeze);
		GetCharacterMovement()->MaxWalkSpeed    = speed * i;
		GetCharacterMovement()->MaxAcceleration = speed * i * 100;
	}
}

bool AEntity::HasEffect(Effect value) {
	return static_cast<uint8>(effect) & static_cast<uint8>(value);
}
void AEntity::AddEffect(Effect value, float strength, float duration) {
	if (static_cast<uint8>(effectImmunity) & static_cast<uint8>(value)) return;
	effect |= static_cast<uint8>(value);
	AdjustEffectStrength(value, strength);
	AdjustEffectDuration(value, duration);
}
void AEntity::RemoveEffect(Effect value) {
	if (!HasEffect(value)) return;
	effect &= ~static_cast<uint8>(value);
	AdjustEffectStrength(value, -EffectStrengthMax);
	AdjustEffectDuration(value, -EffectDurationMax);
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
	case Effect::Slowness:    strength = FMath::Clamp(strength, 0.0f, EffectStrengthMax); break;
	case Effect::Burn:        strength = FMath::Clamp(strength, 0.0f, EffectStrengthMax); break;
	case Effect::Stun:        strength = FMath::Clamp(strength, 1.0f, 1.0f);              break;
	case Effect::Freeze:      strength = FMath::Clamp(strength, 0.0f, 1.0f);              break;
	}
	switch (value) {
	case Effect::HealthBoost: break;
	case Effect::DamageBoost: break;
	case Effect::Resistance:  break;
	case Effect::Speed:       refreshSpeed = true; break;
	case Effect::Slowness:    refreshSpeed = true; break;
	case Effect::Burn:        refreshColor = true; refreshSpeed = true; break;
	case Effect::Stun:        refreshColor = true; break;
	case Effect::Freeze:      refreshColor = true; refreshSpeed = true; break;
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
	case Effect::Slowness:    duration = FMath::Clamp(duration, 0.0f, EffectDurationMax); break;
	case Effect::Burn:        duration = FMath::Clamp(duration, 0.0f, EffectDurationMax); break;
	case Effect::Stun:        duration = FMath::Clamp(duration, 0.0f, EffectDurationMax); break;
	case Effect::Freeze:      duration = FMath::Clamp(duration, 0.0f, EffectDurationMax); break;
	}
	effectDuration[GetIndex(value)] = duration;
	return duration;
}
