#include "StageTrigger.h"
#include "Ghost.h"
#include "Creature.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AStageTrigger::AStageTrigger() {
	defaultHitboxRadius = 320.0f;
	defaultHitboxHeight = 640.0f;
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AStageTrigger::OnStart() {
	Super::OnStart();
}
void AStageTrigger::OnSpawn() {
	Super::OnSpawn();

	SetSpriteIndex(nullptr, 63);
}
void AStageTrigger::OnDespawn() {
	Super::OnDespawn();
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void AStageTrigger::OnCollision(AEntity* entity) {
	Super::OnCollision(entity);

	if (entity && static_cast<AEntity*>(entity) == GetGhost()->GetPlayer()) {
		UE_LOG(LogTemp, Log, TEXT("stage trigger"));
		Despawn();
	}
}
void AStageTrigger::OnInteract(AEntity* entity) {
	Super::OnInteract(entity);
}
