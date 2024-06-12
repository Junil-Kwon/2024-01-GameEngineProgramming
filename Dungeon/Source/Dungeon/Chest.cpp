#include "Chest.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AChest::AChest() {
	defaultHitboxRadius =  50.0f;
	defaultHitboxHeight = 100.0f;
	defaultTag += static_cast<uint8>(Tag::Interactability);
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AChest::OnStart() {
	Super::OnStart();
}
void AChest::OnSpawn() {
	Super::OnSpawn();

	action = Action::Idle;
	actionDelay = 0.0f;
	loot = 0;
	int32 num = FMath::RandRange(12, 24);
	for (int i = 0; i < num; i++) lootArray.Insert(Identifier::Money, 0);
}
void AChest::OnDespawn() {
	Super::OnDespawn();

	lootArray.Empty();
}

// =============================================================================================================
// Update
// =============================================================================================================

void AChest::Update(float DeltaTime) {
	Super::Update(DeltaTime);

	actionDelay += DeltaTime;
	switch (action) {
	case Action::Idle:
		break;
	case Action::Move:
		SetSpriteIndex(nullptr, FMath::Min( 1 + static_cast<int32>(actionDelay * 10),  7));
		if (actionDelay - DeltaTime == 0.0f) loot = 0;
		if (0.7f + (float(loot) / 30.0f) <= actionDelay && loot < lootArray.Num()) {
			FVector location  = GetActorLocation() + FVector(0.0f, 0.0f, GetHitboxHeight() * 0.2f);
			FVector direction = FVector::ZeroVector;
			float angle = FMath::RandRange(0.6f * PI, 1.4f * PI);
			direction.X = FMath::Cos(angle) * FMath::RandRange(180.0f, 400.0f);
			direction.Y = FMath::Sin(angle) * FMath::RandRange(180.0f, 400.0f);
			direction.Z = FMath::RandRange(180.0f, 240.0f);
			Spawn(lootArray[loot], location)->LaunchCharacter(direction, true, true);
			loot++;
		}
		if (0.7f <= actionDelay && lootArray.Num() <= loot) {
			action = Action::Defeat;
			actionDelay = 0.0f;
		}
		break;
	case Action::Defeat:
		break;
	}
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void AChest::OnInteract(AEntity* entity) {
	Super::OnInteract(entity);

	RemoveTag(Tag::Interactability);
	action = Action::Move;
	actionDelay = 0.0f;
}





// =============================================================================================================
// Chest
// =============================================================================================================

void AChest::AddLoot(Identifier value, int32 num) {
	for (int32 i = 0; i < num; i++) lootArray.Add(value);
}
