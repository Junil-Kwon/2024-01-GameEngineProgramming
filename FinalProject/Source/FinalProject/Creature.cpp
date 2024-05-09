#include "Creature.h"



ACreature::ACreature() {
	effectStrength.Init(0.0f, static_cast<int32>(Effect::Max));
	effectDuration.Init(0.0f, static_cast<int32>(Effect::Max));

	health = healthMax;
}

void ACreature::BeginPlay() {
	Super::BeginPlay();
}

void ACreature::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ACreature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}



float ACreature::GetHealth() { return health; }
void  ACreature::SetHealth(float value) { health = value; }
void  ACreature::AdjustHealth(float value) {
	health = health + value < 0 ? 0 : (healthMax < health + value ? healthMax : health + value);
	if (health == 0) Die();
}

void ACreature::Die() {
}
