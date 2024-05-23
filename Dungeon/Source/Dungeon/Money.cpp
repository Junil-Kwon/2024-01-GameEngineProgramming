#include "Money.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AMoney::AMoney() {
	defaultHitboxRadius = 24.0f;
	defaultHitboxHeight = 60.0f;
	defaultTag += static_cast<uint8>(Tag::Collectable);
}
void AMoney::BeginPlay() {
	Super::BeginPlay();
	SetCollisionProfileName(TEXT("Particle"));

	SetSpriteIndex(nullptr, int32(FMath::RandRange(0.0f, 19.999f)));
	SetSpriteIntensity(nullptr, 0.2f);
}
