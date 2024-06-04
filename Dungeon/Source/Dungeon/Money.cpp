#include "Money.h"
#include "Ghost.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AMoney::AMoney() {
	defaultHitboxRadius = 24.0f;
	defaultHitboxHeight = 56.0f;
	defaultTag += static_cast<uint8>(Tag::Collectable);
	SetCollisionProfileName(TEXT("Particle"));
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AMoney::OnStart() {
	Super::OnStart();
}
void AMoney::OnSpawn() {
	Super::OnSpawn();

	SetSpriteIndex(nullptr, FMath::RandRange(0, 3));
	SetSpriteIntensity(nullptr, 0.5f);

	moneyValue = MoneyValueMin;
	mergeUpper = FMath::RandRange(MoneyValueMin, MoneyValueMax);
	merged = false;
}
void AMoney::OnDespawn() {
	Super::OnDespawn();
}

// =============================================================================================================
// Update
// =============================================================================================================

void AMoney::Update(float DeltaTime) {
	Super::Update(DeltaTime);

	if (merged) Despawn();
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void AMoney::OnCollision(AEntity* entity) {
	Super::OnCollision(entity);

	if (mergeUpper <= moneyValue) return;
	AMoney* money = static_cast<AMoney*>(entity);
	if (entity && entity->IsA(AMoney::StaticClass()) && !merged && !money->merged) {
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

	if (merged) return;
	int32 num = FMath::RandRange(1, 2);
	if (entity != nullptr) for (uint8 i = 0; i < num; i++) {
		FVector location = FVector::ZeroVector;
		float angle = FMath::RandRange(0.0f * PI, 2.0f * PI);
		location.X = -4.0f;
		location.Y = entity->GetHitboxRadius() * FMath::Cos(angle) * FMath::RandRange(0.0f, 0.8f);
		location.Z = entity->GetHitboxHeight() * FMath::Sin(angle) * FMath::RandRange(0.0f, 0.4f);
		Spawn(Identifier::Twinkle, entity->GetActorLocation() + RotateVector(location))->Attach(entity);
	}
	GetGhost()->AdjustMoney(moneyValue);
	Despawn();
}





// =============================================================================================================
// Money
// =============================================================================================================

void AMoney::SetValue(int32 value) {
	moneyValue = FMath::Clamp(value, MoneyValueMin, MoneyValueMax);
	SetSpriteIndex(nullptr, (moneyValue - 1) * 4 + FMath::RandRange(0, 3));
	if (moneyValue == MoneyValueMax) SetHitbox(GetHitboxRadius() * 1.5f, GetHitboxHeight() * 1.5f);
}
