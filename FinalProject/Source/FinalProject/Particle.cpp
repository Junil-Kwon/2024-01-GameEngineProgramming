#include "Particle.h"



AParticle::AParticle() {
	defaultTag += 1 << GetIndex(Tag::Floating);
	defaultTag += 1 << GetIndex(Tag::Piercing);
	SetHitbox(0.0f, 0.0f);
}

void AParticle::BeginPlay() {
	Super::BeginPlay();
}



bool AParticle::UpdateAction(float DeltaTime) {
	if (!Super::UpdateAction(DeltaTime)) return false;

	switch (GetIdentifier()) {
	case Identifier::Dust:
		SetSpriteIndex(nullptr, FMath::Min(0 + static_cast<int32>(actionDelay * 10), 5));
		if (0.5f < actionDelay) Destroy();
		break;
	}
	return true;
}
