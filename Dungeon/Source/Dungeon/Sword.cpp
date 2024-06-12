#include "Sword.h"
#include "Creature.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

ASword::ASword() {
	defaultAttackDamage =   0.0f;
	defaultDefendDamage =   0.0f;
	defaultWeaponRange  = 120.0f;
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void ASword::OnStart() {
	Super::OnStart();
}
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

	float angle = !parent ? 0.0f : ToAngle(parent->GetLookDirection());
	FVector location = GetAngleLocation(angle);
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
			SetActorLocation(location);
			SetSpriteXFlip(nullptr, false);
			SetSpriteAngle(nullptr, angle - 45.0f);

			if (parent->SetAction(parent->GetSprite())) parent->SetActionDelay(parent->GetSpriteDelay());
			parent->SetActionCooldown(Action::Attack, 0.4f);
			if (!parent->IsFalling()) parent->AddEffect(Effect::Speed, 0.5f, 0.1f);
			location = parent->GetActorLocation();
			location += parent->GetLookDirection() * (parent->GetHitboxRadius() * 0.5f + GetWeaponRange() / 2);
			parent->Melee(location, GetWeaponRange() / 2, GetAttackDamage());
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
			Damage(this, 0.0f);

			if (parent->SetAction(parent->GetSprite())) parent->SetActionDelay(parent->GetSpriteDelay());
			parent->SetActionCooldown(Action::Defend, 1.0f);
			parent->AddEffect(Effect::Resistance, 0.8f, 0.5f);
			parent->AddEffect(Effect::Slowness,   0.5f, 0.5f);
		}
		if (0.5f <= GetActionDelay()) {
			SetAction(Action::Idle);
		}
		break;
	}
}
