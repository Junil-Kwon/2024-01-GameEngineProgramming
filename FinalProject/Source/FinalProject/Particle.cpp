#include "Particle.h"



AParticle::AParticle() {
	AddTag(Tag::Floating);
	AddTag(Tag::Piercing);
	SetHitbox(0.0f, 0.0f);
}

void AParticle::BeginPlay() {
	Super::BeginPlay();
}



bool AParticle::UpdateAction(float DeltaTime) {
	if (!Super::UpdateAction(DeltaTime)) return false;

	switch (GetIdentifier()) {
	case Identifier::Dust:

		break;
	}
	return true;
}
