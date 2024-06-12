#include "Tomahawk.h"
#include "Creature.h"

#include "GameFramework/CharacterMovementComponent.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

ATomahawk::ATomahawk() {
	defaultAttackDamage =   0.0f;
	defaultDefendDamage =   0.0f;
	defaultWeaponRange  = 136.0f;
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void ATomahawk::OnStart() {
	Super::OnStart();
}
void ATomahawk::OnSpawn() {
	Super::OnSpawn();
}
void ATomahawk::OnDespawn() {
	Super::OnDespawn();
}





// =============================================================================================================
// Action
// =============================================================================================================

bool ATomahawk::VerifyAction(Action value) {
	if (value == Action::Attack && (parent && parent->GetActionCooldown(Action::Defend))) return false;
	return true;
}
void ATomahawk::UpdateAction(float DeltaTime) {
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
		case 0: SetSpriteIndex(nullptr, FMath::Min(1 + int32(GetActionDelay() * 20),  6)); break;
		case 1: SetSpriteIndex(nullptr, FMath::Min(7 + int32(GetActionDelay() * 20), 12)); break;
		}
		if (GetActionDelay() - DeltaTime == 0.0f) {
			SetActorLocation(location);
			SetSpriteXFlip(nullptr, false);
			SetSpriteAngle(nullptr, ToAngle(parent->GetLookDirection()) - 45.0f);

			if (parent->SetAction(parent->GetSprite())) parent->SetActionDelay(parent->GetSpriteDelay());
			parent->SetActionCooldown(Action::Attack, 0.6f);
			location = parent->GetActorLocation();
			location += parent->GetLookDirection() * (parent->GetHitboxRadius() * 0.5f + GetWeaponRange() / 2);
			parent->Melee(location, GetWeaponRange() / 2, GetDefendDamage());
		}
		if (0.5f <= GetActionDelay()) SetAction(Action::Idle);
		break;
	case Action::Defend:
		if (GetActionDelay() - DeltaTime == 0.0f) {
			AEntity* thrown = Spawn(Identifier::TomahawkThrown, location);
			thrown->SetSpriteXFlip(nullptr, GetSpriteXFlip());
			thrown->SetGroup(GetGroup());
			thrown->AddEffect(Effect::DamageBoost, GetAttackDamage() * 2.5f);
			thrown->LaunchCharacter(parent->GetLookDirection() * 4096.0f, true, true);
			
			if (parent->SetAction(parent->GetSprite())) parent->SetActionDelay(parent->GetSpriteDelay());
			parent->SetActionCooldown(Action::Defend, 1.0f);
			parent->AddEffect(Effect::Slowness, 0.2f, 0.3f);
			parent->SetWeapon(nullptr);
			Despawn();
		}
		break;
	}
}
