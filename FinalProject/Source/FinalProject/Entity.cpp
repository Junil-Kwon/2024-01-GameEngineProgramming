#include "Entity.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"



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



bool AEntity::operator==(const AEntity& other) const { return this == &other; }

UStaticMesh* AEntity::GetPlaneMesh() {
	static FString Path = TEXT("/Engine/BasicShapes/Plane.Plane");
	static ConstructorHelpers::FObjectFinder<UObject> Object(*Path);
	return static_cast<UStaticMesh*>(Object.Object);
}
UStaticMesh* AEntity::GetSphereMesh() {
	static FString Path = TEXT("/Engine/BasicShapes/Sphere.Sphere");
	static ConstructorHelpers::FObjectFinder<UObject> Object(*Path);
	return static_cast<UStaticMesh*>(Object.Object);
}
UMaterialInstance* AEntity::GetMaterialInstance() {
	static FString Path = TEXT("/Game/Materials/MaterialInstance.MaterialInstance");
	static ConstructorHelpers::FObjectFinder<UObject> Object(*Path);
	return static_cast<UMaterialInstance*>(Object.Object);
}
AEntity* AEntity::Spawn(Identifier value, FVector location) {
	//static Hero
	//static ConstructorHelpers::FObjectFinder<UObject> Indicator(TEXT("/Game/Blueprints/BP_Indicator.BP_Indicator_C"));
	//static ConstructorHelpers::FObjectFinder<UObject> Dust     (TEXT("/Game/Blueprints/BP_Particle_Dust.BP_Particle_Dust_C"));
	
	UObject* obj = nullptr;
	switch (value) {
	case Identifier::Hero:			obj = nullptr;				break;
	//case Identifier::Indicator:		obj = Indicator.Object;		break;
	//case Identifier::Dust:			obj = Dust.Object;			break;
	default:						obj = nullptr;				break;
	}

	if (obj == nullptr) return nullptr;
	return GetWorld()->SpawnActor<AEntity>(static_cast<UClass*>(obj), location, FRotator::ZeroRotator);
}



AEntity::AEntity() {
 	PrimaryActorTick.bCanEverTick = true;
	Spawn(Identifier::None);

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
	spriteComponent->SetupAttachment(hitboxComponent);
}

void AEntity::BeginPlay() {
	Super::BeginPlay();

	yPrev = GetActorLocation().Y;

	SetHitbox(hitboxRadius, hitboxHeight);
	hitboxComponent->GetBodyInstance()->bLockXRotation = true;
	hitboxComponent->GetBodyInstance()->bLockYRotation = true;
	hitboxComponent->GetBodyInstance()->bLockZRotation = true;
	hitboxComponent->GetBodyInstance()->MassScale = mass;

	UTexture* texture = nullptr;
	materialInstance->GetTextureParameterValue(TEXT("Texture"), texture);
	float xSize = Cast<UTexture2D>(texture)->GetSizeX();
	float ySize = Cast<UTexture2D>(texture)->GetSizeX();
	spriteCount = xSize / ySize;
	spriteComponent->SetWorldScale3D(FVector::OneVector * ySize * 0.04f);
	spriteComponent->SetMaterial(0, materialInstance);

	shadowComponent->SetVisibility(true);
	shadowComponent->bCastHiddenShadow = true;
	shadowComponent->bHiddenInGame = true;

	SetState(state);

	SetGroup(group);

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

	float yCurr = GetActorLocation().Y;
	if (yCurr < yPrev) {
		isFalling = true;
		fallSpeed = -GetVelocity().Y;
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
	yPrev = yCurr;

	UpdateSprite(DeltaTime);
	UpdateEffect(DeltaTime);
}
bool AEntity::IsFalling() { return isFalling; }



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
	FVector scale = FVector(hitboxRadius * 2, hitboxRadius * 2, hitboxHeight) * 0.01f;
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



int32 AEntity::GetSpriteCount() { return spriteCount; }
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



State AEntity::GetState() {
	return state;
}
void AEntity::SetState(State value) {
	state = value;
	switch (value) {
	case State::Healthy: break;
	case State::Dead:    break;
	}
}



Group AEntity::GetGroup() {
	return group;
}
void AEntity::SetGroup(Group value) {
	group = value;
}



bool AEntity::HasTag(Tag value) {
	return tag & ToInt(value);
}
bool AEntity::AddTag(Tag value) {
	if (HasTag(value)) return false;
	tag |= ToInt(value);
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
	tag &= ~ToInt(value);
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
	return (ToInt(effect) & ToInt(value)) != 0;
}
bool AEntity::AddEffect(Effect value, float strength, float duration) {
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
