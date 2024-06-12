#include "Particle.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AParticle::AParticle() {
	defaultTag += static_cast<uint8>(Tag::Floating);
	defaultTag += static_cast<uint8>(Tag::Piercing);
	SetCollisionProfileName(TEXT("Particle"));
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AParticle::OnStart() {
	Super::OnStart();
}
void AParticle::OnSpawn() {
	Super::OnSpawn();
}
void AParticle::OnDespawn() {
	Super::OnDespawn();
}

// =============================================================================================================
// Update
// =============================================================================================================

void AParticle::Update(float DeltaTime) {
	Super::Update(DeltaTime);
	
	switch (GetIdentifier()) {
	case Identifier::Dust:
	case Identifier::Flame:
	case Identifier::DebuffDust:
	case Identifier::FireballDust:
	case Identifier::IcicleDust:
		if (GetLifeTime() - DeltaTime == 0.0f) SetSpriteXFlip(nullptr, FMath::RandBool());
		SetSpriteIndex(nullptr, FMath::Min(0 + static_cast<int32>(GetLifeTime() * 10), 4));
		if (0.5f <= GetLifeTime()) Despawn();
		break;

	case Identifier::Twinkle:
	case Identifier::HealthUp:
	case Identifier::EnergyUp:
		SetSpriteIndex(nullptr, FMath::Min(0 + static_cast<int32>(GetLifeTime() * 10), 4));
		if (GetLifeTime() - DeltaTime == 0.0) SetSpriteIntensity(nullptr, 0.2f);
		if (0.5f <= GetLifeTime()) Despawn();
		break;

	case Identifier::Lightning:
		if (GetLifeTime() - DeltaTime == 0.0f) {
			SetSpriteXFlip(nullptr, FMath::RandBool());
			SetSpriteIntensity(nullptr, 1.0f);
		}
		SetSpriteIndex(nullptr, FMath::Min(0 + static_cast<int32>(GetLifeTime() * 10), 8));
		if (1.0f <= GetLifeTime()) Despawn();
		break;
	}
}

