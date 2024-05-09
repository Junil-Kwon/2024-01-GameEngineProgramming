#include "Entity.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"



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

	GetCapsuleComponent()->InitCapsuleSize(36.0f, 60.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Entity"));

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
	
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->GravityScale = HasTag(Tag::Floating) ? 0.0f : 2.0f;
	GetCharacterMovement()->MaxWalkSpeed = speed;
	GetCharacterMovement()->MaxAcceleration = speed * 100;
}

void AEntity::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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
