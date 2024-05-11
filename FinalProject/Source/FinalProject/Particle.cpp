#include "Particle.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"



AParticle::AParticle() {
	PrimaryActorTick.bCanEverTick = true;

	hitboxComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitboxComponent"));
	hitboxComponent->SetCollisionProfileName(TEXT("Projectile"));
	hitboxComponent->InitCapsuleSize(0.0f, 0.0f);
	hitboxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(hitboxComponent);

	spriteComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpriteComponent"));
	spriteComponent->SetWorldRotation(FRotator(0.0f, 90.0f, 41.409618f));
	spriteComponent->SetWorldScale3D(FVector(1.28f, 1.28f, 1.28f));
	spriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	spriteComponent->SetupAttachment(hitboxComponent);
}



void AParticle::BeginPlay() {
	Super::BeginPlay();

	hitboxRadius = hitboxComponent->GetUnscaledCapsuleRadius();
	hitboxHeight = hitboxComponent->GetUnscaledCapsuleHalfHeight() * 2.0f;
	ECollisionEnabled::Type type = ECollisionEnabled::QueryAndPhysics;
	if (!hitboxRadius || !hitboxHeight) ECollisionEnabled::QueryOnly;
	hitboxComponent->SetCollisionEnabled(type);

	spriteScale = spriteComponent->GetRelativeScale3D().X / 1.28f;
}



void AParticle::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	UpdateSprite(DeltaTime);
}



float AParticle::GetHitboxRadius() { return hitboxRadius; }
float AParticle::GetHitboxHeight() { return hitboxHeight; }
bool  AParticle::SetHitboxRadius(float value) {
	if (hitboxRadius == value) return false;
	hitboxRadius = value;
	hitboxComponent->SetCapsuleSize(value, hitboxHeight);
	ECollisionEnabled::Type type = ECollisionEnabled::QueryAndPhysics;
	if (!hitboxRadius || !hitboxHeight) ECollisionEnabled::QueryOnly;
	hitboxComponent->SetCollisionEnabled(type);
	return true;
}
bool  AParticle::SetHitboxHeight(float value) {
	if (hitboxHeight == value) return false;
	hitboxHeight = value;
	hitboxComponent->SetCapsuleSize(hitboxRadius, value);
	ECollisionEnabled::Type type = ECollisionEnabled::QueryAndPhysics;
	if (!hitboxRadius || !hitboxHeight) ECollisionEnabled::QueryOnly;
	hitboxComponent->SetCollisionEnabled(type);
	return true;
}
bool  AParticle::SetHitbox(float radius, float height) {
	if (hitboxRadius == radius && hitboxHeight == height) return false;
	hitboxRadius = radius;
	hitboxHeight = height;
	hitboxComponent->SetCapsuleSize(radius, height);
	ECollisionEnabled::Type type = ECollisionEnabled::QueryAndPhysics;
	if (!hitboxRadius || !hitboxHeight) ECollisionEnabled::QueryOnly;
	hitboxComponent->SetCollisionEnabled(type);
	return true;
}
FVector AParticle::GetFootLocation() {
	return GetActorLocation() + FVector(-hitboxHeight * 0.5f, 0.0f, -hitboxHeight * 0.5f + 64.0f);
}

int32 AParticle::GetSpriteIndex() { return spriteIndex; }
float AParticle::GetSpriteScale() { return spriteScale; }
bool  AParticle::GetSpriteXFlip() { return spriteXflip; }
bool  AParticle::SetSpriteIndex(int32 value) {
	if (spriteIndex == value) return false;
	spriteIndex = value;
	spriteComponent->SetScalarParameterValueOnMaterials(TEXT("Index"), value);
	return true;
}
bool AParticle::SetSpriteScale(float value) {
	if (spriteScale == value) return false;
	spriteScale = value;
	spriteComponent->SetRelativeScale3D(FVector(1.28f, 1.28f, 1.28f) * spriteScale);
	return true;
}
bool  AParticle::SetSpriteXFlip(bool value) {
	if (spriteXflip == value) return false;
	spriteXflip = value;
	spriteComponent->SetScalarParameterValueOnMaterials(TEXT("XFlip"), value ? 1.0f : 0.0f);
	return true;
}
void AParticle::UpdateSprite(float DeltaTime) {
	spriteDelay += DeltaTime;
	switch (identifier) {
	case Identifier::Dust: if (0.5f < spriteDelay) Destroy(); break;
	}

	int32 i = 0;
	switch (identifier) {
	case Identifier::Dust: i = 1 + static_cast<int32>(spriteDelay * 10); if (5 < i) i = 5; break;
	}
	if (spriteIndex == i) return;
	spriteIndex = i;
	spriteComponent->SetScalarParameterValueOnMaterials(TEXT("Index"), spriteIndex);
}