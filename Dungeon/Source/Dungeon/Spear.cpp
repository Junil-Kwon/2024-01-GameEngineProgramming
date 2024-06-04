#include "Spear.h"
#include "Creature.h"

#include "GameFramework/CharacterMovementComponent.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

ASpear::ASpear() {
	defaultAttackDamage =   0.0f;
	defaultDefendDamage =   0.0f;
	defaultWeaponRange  = 200.0f;
	defaultDroppable    = false;
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void ASpear::OnStart() {
	Super::OnStart();
}
void ASpear::OnSpawn() {
	Super::OnSpawn();
}
void ASpear::OnDespawn() {
	Super::OnDespawn();
}





// =============================================================================================================
// Action
// =============================================================================================================

bool ASpear::VerifyAction(Action value) {
	if (value == Action::Attack && (parent && parent->GetActionCooldown(Action::Defend))) return false;
	return true;
}
void ASpear::UpdateAction(float DeltaTime) {
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
		SetSpriteIndex(nullptr, FMath::Min(1 + int32(GetActionDelay() * 20), 4));
		if (GetActionDelay() - DeltaTime == 0.0f) {
			SetActorLocation(location);
			SetSpriteXFlip(nullptr, false);
			SetSpriteAngle(nullptr, angle - 45.0f);

			if (parent->SetAction(parent->GetSprite())) parent->SetActionDelay(parent->GetSpriteDelay());
			parent->SetActionCooldown(Action::Attack, 0.4f);
			location = parent->GetActorLocation();
			location += parent->GetLookDirection() * parent->GetHitboxRadius() * 0.5f;
			for (int32 i = 0; i < 5; i++) {
				location += parent->GetLookDirection() * i * 20.0f;
				parent->Melee(location, 10.0f, GetAttackDamage());
			}
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
			if (parent->SetAction(parent->GetSprite())) parent->SetActionDelay(parent->GetSpriteDelay());
			parent->SetActionCooldown(Action::Defend, 0.25f);
			SetAction(Action::Idle);
		}
		break;
	}
}
