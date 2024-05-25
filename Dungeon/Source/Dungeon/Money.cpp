#include "Money.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AMoney::AMoney() {
	defaultHitboxRadius = 24.0f;
	defaultHitboxHeight = 48.0f;
	defaultTag += static_cast<uint8>(Tag::Collectable);
	SetCollisionProfileName(TEXT("Money"));
}

// =============================================================================================================
// Object Pool
// =============================================================================================================

void AMoney::OnSpawn() {
	Super::OnSpawn();

	SetSpriteIndex(nullptr, FMath::RandRange(0, 3));
	SetSpriteIntensity(nullptr, 0.1f);

	moneyValue = 1;
	mergeUpper = FMath::RandRange(1, ValueMax);
	merged = false;
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void AMoney::OnCollision(AEntity* entity) {
	Super::OnCollision(entity);

	if (mergeUpper <= moneyValue) return;
	AMoney* money = static_cast<AMoney*>(entity);
	if (entity->IsA(AMoney::StaticClass()) && !merged && !money->merged) {
		int32 mergeValue = moneyValue + money->moneyValue;
		if (mergeValue <= mergeUpper && mergeValue <= money->mergeUpper) {
			Spawn(Identifier::Twinkle, money->GetActorLocation());
			SetValue(mergeValue);
			money->merged = true;
		}
	}
}
void AMoney::OnInteract(AEntity* entity) {
	Super::OnInteract(entity);

	int32 num = FMath::RandRange(1, 2);
	if (entity != nullptr) for (uint8 i = 0; i < num; i++) {
		FVector location = FVector::ZeroVector;
		float angle = FMath::RandRange(0.0f * PI, 2.0f * PI);
		location.X = -4.0f;
		location.Y = entity->GetHitboxRadius() * FMath::Cos(angle) * FMath::RandRange(0.6f, 0.8f);
		location.Z = entity->GetHitboxHeight() * FMath::Sin(angle) * FMath::RandRange(0.3f, 0.4f);
		Spawn(Identifier::Twinkle, entity->GetActorLocation() + RotateVector(location))->Attach(entity);
	}
	// add money to player
	// GetGhost()->AddMoney(moneyValue);
	Despawn();
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

bool AMoney::UpdateAction(float DeltaTime) {
	if (!Super::UpdateAction(DeltaTime)) return false;
	if (merged) Despawn();

	return false;
}





// =============================================================================================================
// Money
// =============================================================================================================

void AMoney::SetValue(int32 value) {
	moneyValue = FMath::Clamp(value, 1, ValueMax);
	SetSpriteIndex(nullptr, (moneyValue - 1) * 4 + FMath::RandRange(0, 3));
	if (moneyValue == ValueMax) SetHitbox(GetHitboxRadius() * 1.5f, GetHitboxHeight() * 1.5f);
}
