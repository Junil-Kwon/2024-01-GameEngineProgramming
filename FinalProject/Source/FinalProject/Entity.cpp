#include "Entity.h"
#include "Particle.h"
#include "Indicator.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <type_traits>



uint8 GetIndex(Tag tag) {
	uint8 length = ToInt(Tag::Max);
	for (uint8 i = 0; i < length; i++) if ((ToInt(tag) & (1 << i)) != 0) return i;
	return -1;
}
uint8 GetIndex(Effect effect) {
	uint8 length = ToInt(Effect::Max);
	for (uint8 i = 0; i < length; i++) if ((ToInt(effect) & (1 << i)) != 0) return i;
	return -1;
}
template <typename T>
T ToEnum(uint8 integer) {
	return static_cast<T>(integer);
}
template <typename T>
uint8 ToInt(T enumerate) {
	return static_cast<uint8>(enumerate);
}

FRotator VectorToRotator(FVector vector) {
	return FRotator(0.0f, 90.0f - FMath::RadiansToDegrees(FMath::Atan2(vector.X, vector.Y)), 0.0f);
}
FVector RotatorToVector(FRotator rotator) {
	float rad = FMath::DegreesToRadians(rotator.Yaw);
	return FVector(FMath::Cos(rad), FMath::Sin(rad), 0.0f);
}



AEntity::AEntity() {
 	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AParticle> ParticleClass(TEXT("/Game/Blueprints/BP_Particle_Dust.BP_Particle_Dust_C"));
	if (ParticleClass.Succeeded()) particleClass = ParticleClass.Class;
	static ConstructorHelpers::FClassFinder<AIndicator> IndicatorClass(TEXT("/Game/Blueprints/BP_Indicator.BP_Indicator_C"));
	if (IndicatorClass.Succeeded()) indicatorClass = IndicatorClass.Class;

	shadowComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShadowComponent"));
	shadowComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	shadowComponent->bCastHiddenShadow = true;
	shadowComponent->bHiddenInGame = true;
	shadowComponent->SetupAttachment(RootComponent);

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	meshComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 41.409618f));
	meshComponent->SetRelativeScale3D(FVector(1.28f, 1.28f, 1.28f));
	meshComponent->SetupAttachment(RootComponent);
}

void AEntity::BeginPlay() {
	Super::BeginPlay();
	
	GetCharacterMovement()->JumpZVelocity = 800.0f;
	GetCharacterMovement()->GravityScale = HasTag(Tag::Floating) ? 0.0f : 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = speed;
	GetCharacterMovement()->MaxAcceleration = speed * 100;

	hitboxRadius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	hitboxHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() * 2.0f;
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
			Spawn<AParticle>(GetFootLocation() + FVector(0.0f, -hitboxRadius *  0.75f, 0.0f));
			Spawn<AParticle>(GetFootLocation() + FVector(0.0f, -hitboxRadius * -0.75f, 0.0f));
		}
		isFalling = false;
		fallSpeed = 0.0f;
	}

	for (uint8 i = 0; i < ToInt(Effect::Max); i++) {
	}
	UpdateEffect();

	spriteDelay += DeltaTime;
	UpdateSprite();
}



template <typename T> T* AEntity::Spawn(FVector Location) {
	if constexpr (std::is_base_of<AIndicator, T>::value) {
		return GetWorld()->SpawnActor<T>(indicatorClass, Location, FRotator::ZeroRotator);
	}
	if constexpr (std::is_base_of<AParticle, T>::value) {
		return GetWorld()->SpawnActor<T>(particleClass, Location, FRotator::ZeroRotator);
	}
}



bool AEntity::HasTag(Tag value = ToEnum<Tag>(~0)) {
	return (ToInt(tag) & ToInt(value)) != 0;
}
void AEntity::AddTag(Tag value) {
	tag = ToEnum<Tag>(ToInt(tag) | ToInt(value));
}
void AEntity::RemoveTag(Tag value) {
	tag = ToEnum<Tag>(ToInt(tag) & ~ToInt(value));
}

bool AEntity::HasEffect(Effect value = ToEnum<Effect>(~0)) {
	return (ToInt(effect) & ToInt(value)) != 0;
}
bool AEntity::AddEffect(Effect value, float strength = 1.0f, float duration = 1.0f) {
	if ((ToInt(effectImmunity) & ToInt(value)) != 0) return false;
	effect = ToEnum<Effect>(ToInt(effect) | ToInt(value));
	return true;
}
void AEntity::RemoveEffect(Effect value) {
	effect = ToEnum<Effect>(ToInt(effect) & ~ToInt(value));
}

void AEntity::UpdateEffect() {
	for (uint8 i = 0; i < ToInt(Effect::Max); i++) {
	}
}

void AEntity::UpdateSprite() {
}



FVector AEntity::GetFootLocation() {
	return GetActorLocation() + FVector(-hitboxHeight * 0.5f, 0.0f, -hitboxHeight * 0.5f + 64.0f);
}