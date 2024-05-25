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
// Action
// =============================================================================================================

bool AParticle::UpdateAction(float DeltaTime) {
	if (!Super::UpdateAction(DeltaTime)) return false;
	
	if (actionDelay - DeltaTime == 0.0f) SetSpriteXFlip(nullptr, FMath::RandBool());
	switch (GetIdentifier()) {

	case Identifier::Dust:
	case Identifier::Flame:
		SetSpriteIndex(nullptr, FMath::Min(0 + static_cast<int32>(actionDelay * 10), 4));
		if (0.5f <= actionDelay) Despawn();
		break;

	case Identifier::Twinkle:
		SetSpriteIndex(nullptr, FMath::Min(0 + static_cast<int32>(actionDelay * 10), 4));
		if (actionDelay - DeltaTime == 0.0) SetSpriteIntensity(nullptr, 0.2f);
		if (0.5f <= actionDelay) Despawn();
		break;

	}
	return true;
}

