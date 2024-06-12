#include "Projectile.h"
#include "Creature.h"

#include "GameFramework/CharacterMovementComponent.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AProjectile::AProjectile() {
	defaultHitboxRadius = 10.0f;
	defaultHitboxHeight = 20.0f;
	SetCollisionProfileName(TEXT("Projectile"));
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AProjectile::OnStart() {
	Super::OnStart();
}
void AProjectile::OnSpawn() {
	Super::OnSpawn();

	switch (GetIdentifier()) {
	case Identifier::TomahawkThrown:
		SetHitbox(24.0f, 48.0f);
		break;
	case Identifier::Debuff:
		SetHitbox(30.0f, 60.0f);
		AddTag(Tag::Floating);
		AddTag(Tag::Piercing);
		break;
	case Identifier::Fireball:
		AddEffect(Effect::Burn, 10.0f);
	case Identifier::Icicle:
		AddTag(Tag::Floating);
		break;
	}
}
void AProjectile::OnDespawn() {
	Super::OnDespawn();

	switch (GetIdentifier()) {
		case Identifier::TomahawkThrown:
			Spawn(Identifier::Tomahawk, GetActorLocation());
			break;
		case Identifier::Debuff:
			Spawn(Identifier::DebuffDust, GetActorLocation());
			break;
		case Identifier::Fireball:
			Spawn(Identifier::FireballDust, GetActorLocation());
			break;
		case Identifier::Icicle:
			Spawn(Identifier::IcicleDust, GetActorLocation());
			break;
	}
}

// =============================================================================================================
// Update
// =============================================================================================================

void AProjectile::Update(float DeltaTime) {
	Super::Update(DeltaTime);

	switch (GetIdentifier()) {
		break;
	case Identifier::Arrow:
		if (3.0f <= GetLifeTime()) Despawn();
		break;
	case Identifier::TomahawkThrown:
		if (parent == nullptr) SetSpriteIndex(nullptr, int32(GetLifeTime() * 40) % 4);
		if (3.0f <= GetLifeTime()) Despawn();
		break;
	case Identifier::Debuff:
		SetSpriteIndex(nullptr, FMath::Min(int32(GetLifeTime() * 40), 3));
		if (5.0f <= GetLifeTime()) Despawn();
		break;
	case Identifier::Fireball:
		SetSpriteIndex(nullptr, int32(GetLifeTime() * 40) % 4);
		break;
	}
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void AProjectile::OnCollision(AEntity* entity) {
	Super::OnCollision(entity);

	if (GetGroup() == Group::None) return;
	float strength;
	float duration;
	ACreature* creature = static_cast<ACreature*>(entity);
	switch (GetIdentifier()) {
	case Identifier::Arrow:
		if (parent == nullptr) {
			SetLifeTime(0.0f);
			if (entity && GetGroup() != entity->GetGroup()) {
				parent = entity;
				Attach(entity);
				AddTag(Tag::Floating);
				AddTag(Tag::Piercing);
				if (entity->IsA(ACreature::StaticClass())) {
					creature->Damage(creature, GetEffectStrength(Effect::DamageBoost));
				}
			}
			else if (!entity) {
				parent = this;
				GetCharacterMovement()->StopMovementImmediately();
				AddTag(Tag::Floating);
				AddTag(Tag::Piercing);
			}
		}
		break;
	case Identifier::TomahawkThrown:
		if (parent == nullptr) {
			SetLifeTime(2.0f);
			SetSpriteIndex(nullptr, GetSpriteIndex() + 4);
			if (entity && GetGroup() != entity->GetGroup()) {
				parent = entity;
				Attach(entity);
				AddTag(Tag::Floating);
				AddTag(Tag::Piercing);
				if (entity->IsA(ACreature::StaticClass())) {
					creature->Damage(creature, GetEffectStrength(Effect::DamageBoost));
					creature->AddEffect(Effect::Stun, 1.0f, 0.5f);
				}
			}
			else if (!entity) {
				parent = this;
				GetCharacterMovement()->StopMovementImmediately();
				AddTag(Tag::Floating);
				AddTag(Tag::Piercing);
			}
		}
		break;
	case Identifier::Debuff:
		if (entity && GetGroup() != entity->GetGroup()) {
			if (entity->IsA(ACreature::StaticClass())) {
				creature->Damage(creature, GetEffectStrength(Effect::DamageBoost));
				creature->AddEffect(Effect::Slowness, 2.0f, 3.0f);
			}
			Despawn();
		}
		else if (!entity) {
			Despawn();
		}
		break;
	case Identifier::Fireball:
		if (entity && GetGroup() != entity->GetGroup()) {
			if (entity->IsA(ACreature::StaticClass())) {
				creature->Damage(creature, GetEffectStrength(Effect::DamageBoost));
				strength = FMath::Clamp(0.5f, 0.0f, 1.0f - creature->GetEffectStrength(Effect::Burn));
				duration = FMath::Clamp(0.5f, 0.0f, 3.0f - creature->GetEffectStrength(Effect::Burn));
				creature->AddEffect(Effect::Burn, strength, duration);
			}
			Despawn();
		}
		else if (!entity) {
			Despawn();
		}
		break;
	case Identifier::Icicle:
		if (entity && GetGroup() != entity->GetGroup()) {
			if (entity->IsA(ACreature::StaticClass())) {
				creature->Damage(creature, GetEffectStrength(Effect::DamageBoost));
				strength = FMath::Clamp(0.2f, 0.0f, 0.8f - creature->GetEffectStrength(Effect::Freeze));
				duration = FMath::Clamp(0.5f, 0.0f, 3.0f - creature->GetEffectStrength(Effect::Freeze));
				creature->AddEffect(Effect::Freeze, strength, duration);
			}
			Despawn();
		}
		else if (!entity) {
			Despawn();
		}
		break;
	}
}
