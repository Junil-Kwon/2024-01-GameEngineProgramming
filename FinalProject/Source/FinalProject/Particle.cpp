#include "Particle.h"



AParticle::AParticle() {
	AddTag(Tag::Floating);
	AddTag(Tag::Piercing);
	SetHitbox(0.0f, 0.0f);
}



void AParticle::BeginPlay() {
	Super::BeginPlay();
}



void AParticle::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}



void AParticle::UpdateSprite(float DeltaTime) {
	Super::UpdateSprite(DeltaTime);

}
