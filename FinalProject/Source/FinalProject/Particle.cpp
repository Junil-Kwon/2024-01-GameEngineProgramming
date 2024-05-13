#include "Particle.h"



AParticle::AParticle() {

}



void AParticle::BeginPlay() {
	Super::BeginPlay();

	AddTag(Tag::Floating);
	AddTag(Tag::Piercing);
}



void AParticle::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}



void AParticle::UpdateSprite(float DeltaTime) {
	Super::UpdateSprite(DeltaTime);

}
