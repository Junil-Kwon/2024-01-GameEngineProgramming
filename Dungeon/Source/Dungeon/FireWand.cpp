#include "FireWand.h"
#include "Creature.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AFireWand::AFireWand() {
	defaultAttackDamage =   0.0f;
	defaultDefendDamage =   0.0f;
	defaultWeaponRange  = 420.0f;
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AFireWand::OnStart() {
	Super::OnStart();
}
void AFireWand::OnSpawn() {
	Super::OnSpawn();
}
void AFireWand::OnDespawn() {
	Super::OnDespawn();
}





// =============================================================================================================
// Action
// =============================================================================================================

bool AFireWand::VerifyAction(Action value) {
	if (value == Action::Attack && GetAction() == Action::Defend) return false;
	return true;
}
void AFireWand::UpdateAction(float DeltaTime) {
	Super::UpdateAction(DeltaTime);

	ACreature* creature;
	float strength;
	float duration;
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
		SetActorLocation(location);
		SetSpriteIndex(nullptr, FMath::Min(1 + int32(GetActionDelay() * 20), 4));
		SetSpriteXFlip(nullptr, false);
		SetSpriteAngle(nullptr, angle - 45.0f);

		if (GetActionDelay() - DeltaTime == 0.0f) {
			if (parent->SetAction(parent->GetSprite())) parent->SetActionDelay(parent->GetSpriteDelay());
			parent->SetActionCooldown(Action::Attack, 0.6f);
			if (GetEnergeCost() <= parent->GetEnergy()) {
				parent->AdjustEnergy(-GetEnergeCost());

				AEntity* projectile = Spawn(Identifier::Fireball, location);
				projectile->SetGroup(GetGroup());
				projectile->SetSpriteAngle(nullptr, GetSpriteAngle());
				projectile->AddEffect(Effect::DamageBoost, GetAttackDamage());
				projectile->LaunchCharacter(parent->GetLookDirection() * 2048.0f, true, true);
			}
		}
		if (0.5f <= GetActionDelay()) {
			SetActorLocation(parent->GetHandLocation());
			SetSpriteIndex(nullptr);
			SetSpriteXFlip(nullptr, parent->GetSpriteXFlip());
			SetSpriteAngle(nullptr);
			SetAction(Action::Idle);
		}
		break;
	case Action::Defend:
		if (GetActionDelay() - DeltaTime == 0 && FMath::RandRange(0.0f, 1.0f) < 0.8f) pattern = pattern ? 0 : 1;
		switch (pattern) {
		case 0: SetSpriteIndex(nullptr, FMath::Min( 5 + int32(GetActionDelay() * 20),  8)); break;
		case 1: SetSpriteIndex(nullptr, FMath::Min( 9 + int32(GetActionDelay() * 20), 12)); break;
		}
		if (GetActionDelay() - DeltaTime == 0.0f) {
			SetActorLocation(location);
			SetSpriteXFlip(nullptr, false);
			SetSpriteAngle(nullptr, angle - 45.0f);

			if (parent->SetAction(parent->GetSprite())) parent->SetActionDelay(parent->GetSpriteDelay());
			parent->SetActionCooldown(Action::Defend, 0.6f);
			location = parent->GetActorLocation();
			location += parent->GetLookDirection() * (parent->GetHitboxRadius() * 0.5f + 40.0f);
			parent->Melee(location, 40.0f, GetAttackDamage());
			for (int32 i = 0; i < parent->melee.Num(); i++) {
				if (parent->melee[i] == nullptr) continue;
				creature = parent->melee[i];
				strength = FMath::Clamp(0.2f, 0.0f, 0.8f - creature->GetEffectStrength(Effect::Burn));
				duration = FMath::Clamp(0.5f, 0.0f, 3.0f - creature->GetEffectStrength(Effect::Burn));
				creature->AddEffect(Effect::Burn, strength, duration);
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
	}
}
