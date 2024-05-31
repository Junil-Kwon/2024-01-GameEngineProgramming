#include "Sword.h"
#include "Creature.h"

#include "GameFramework/CharacterMovementComponent.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

ASword::ASword() {
	defaultWeaponRange = 0.0f;
}
void ASword::BeginPlay() {
	Super::BeginPlay();
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void ASword::OnSpawn() {
	Super::OnSpawn();
}
void ASword::OnDespawn() {
	Super::OnDespawn();
}





// =============================================================================================================
// Action
// =============================================================================================================

bool ASword::VerifyAction(Action value) {
	if (value == Action::Attack && GetAction() == Action::Defend) return false;
	return true;
}
void ASword::UpdateAction(float DeltaTime) {
	Super::UpdateAction(DeltaTime);

	FRotator rotation;
	FVector  location;
	switch (GetAction()) {
	case Action::Idle:
		SetSpriteIndex(nullptr);
		SetSpriteAngle(nullptr);
		if (parent == nullptr) {

		}
		else {
			if (GetSpriteXFlip() != parent->GetSpriteXFlip()) {
				SetActorLocation(parent->GetHandLocation());
				SetSpriteXFlip(nullptr, parent->GetSpriteXFlip());
			}
		}
		break;
	case Action::Attack:
		if (GetActionDelay() - DeltaTime == 0 && FMath::RandRange(0.0f, 1.0f) < 0.8f) pattern = pattern ? 0 : 1;
		switch (pattern) {
		case 0: SetSpriteIndex(nullptr, FMath::Min(1 + int32(GetActionDelay() * 20), 4)); break;
		case 1: SetSpriteIndex(nullptr, FMath::Min(5 + int32(GetActionDelay() * 20), 8)); break;
		}
		if (GetActionDelay() - DeltaTime == 0.0f) {
			rotation = ToRotator(parent->GetLookDirection());
			location = parent->GetActorLocation();
			location = location + ToVector(rotation) * (parent->GetHitboxRadius() * 0.5f);
			location += RotateVector(FVector(FMath::Abs(rotation.Yaw) < 60.0f ? 1.0f : -24.0f, 0.0f, 0.0f));
			SetActorLocation(location);
			SetSpriteXFlip(nullptr, false);
			SetSpriteAngle(nullptr, rotation.Yaw - 45.0f);
			parent->SetAction(Action::Idle);
			parent->SetActionCooldown(Action::Attack, 0.4f);
			if (!parent->GetCharacterMovement()->IsFalling()) parent->AddEffect(Effect::Speed, 0.5f, 0.1f);
		}
		if (0.3f <= GetActionDelay()) {
			SetActorLocation(parent->GetHandLocation());
			SetSpriteIndex(nullptr);
			SetSpriteXFlip(nullptr, parent->GetSpriteXFlip());
			SetSpriteAngle(nullptr);
			SetAction(Action::Idle);
		}
		break;
	case Action::Defend:
		if (GetActionDelay() - DeltaTime == 0.0f) {
			SetActorLocation(parent->GetHandLocation());
			SetSpriteIndex(nullptr);
			SetSpriteXFlip(nullptr, parent->GetSpriteXFlip());
			SetSpriteAngle(nullptr, !GetSpriteXFlip() ? -135.0f : 135.0f);
			Damage();
			parent->SetAction(Action::Idle);
			parent->SetActionCooldown(Action::Defend, 1.0f);
			parent->AddEffect(Effect::Resistance, 0.8f, 0.5f);
			parent->AddEffect(Effect::Slowness, 0.5f, 0.5f);
		}
		if (0.5f <= GetActionDelay()) {
			SetAction(Action::Idle);
		}
		break;
	}
}
