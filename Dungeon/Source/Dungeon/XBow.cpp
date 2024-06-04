#include "XBow.h"
#include "Creature.h"

#include "GameFramework/CharacterMovementComponent.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AXBow::AXBow() {
	defaultWeaponRange = 420.0f;
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AXBow::OnStart() {
	Super::OnStart();
}
void AXBow::OnSpawn() {
	Super::OnSpawn();
}
void AXBow::OnDespawn() {
	Super::OnDespawn();
}





// =============================================================================================================
// Action
// =============================================================================================================

bool AXBow::VerifyAction(Action value) {
	if (value == Action::Attack && (parent && parent->GetActionCooldown(Action::Defend))) return false;
	return true;
}
void AXBow::UpdateAction(float DeltaTime) {
	Super::UpdateAction(DeltaTime);

	float angle = !parent ? 0.0f : ToAngle(parent->GetLookDirection());
	FVector location = GetAngleLocation(angle);
	switch (GetAction()) {
	case Action::Idle:
		if (parent == nullptr) {
			SetSpriteIndex(nullptr);
			SetSpriteAngle(nullptr);
		}
		else {
			SetActorLocation(location);
			SetSpriteIndex(nullptr, 5);
			SetSpriteXFlip(nullptr, false);
			SetSpriteAngle(nullptr, ToAngle(parent->GetLookDirection()) - 45.0f);
		}
		break;
	case Action::Attack:
		if (GetActionDelay() - DeltaTime == 0.0f) {
			AEntity* arrow = Spawn(Identifier::Arrow, location);
			arrow->SetGroup(GetGroup());
			arrow->SetSpriteAngle(nullptr, GetSpriteAngle());
			arrow->LaunchCharacter(parent->GetLookDirection() * 4096.0f, true, true);
			
			if (parent->SetAction(parent->GetSprite())) parent->SetActionDelay(parent->GetSpriteDelay());
			parent->SetActionCooldown(Action::Attack, 0.3f);
			parent->AddEffect(Effect::Slowness, 0.2f, 0.3f);
		}
		SetActorLocation(location);
		SetSpriteIndex(nullptr, FMath::Min( 1 + int32(GetActionDelay() * 20), 5));
		SetSpriteXFlip(nullptr, false);
		SetSpriteAngle(nullptr, angle - 45.0f);

		if (0.3f <= GetActionDelay()) {
			SetAction(Action::Idle);
		}
		break;
	case Action::Defend:
		SetSpriteIndex(nullptr, FMath::Min( 6 + int32(GetActionDelay() * 20),  9));
		if (GetActionDelay() - DeltaTime == 0.0f) {
			SetActorLocation(location);
			SetSpriteIndex(nullptr, FMath::Min(5 + int32(GetActionDelay() * 20), 7));
			SetSpriteXFlip(nullptr, false);
			SetSpriteAngle(nullptr, ToAngle(parent->GetLookDirection()) - 45.0f);

			if (parent->SetAction(parent->GetSprite())) parent->SetActionDelay(parent->GetSpriteDelay());
			parent->SetActionCooldown(Action::Defend, 0.6f);
			parent->Melee(location, 80.0f, 1.0f);
		}
		if (0.5f <= GetActionDelay()) SetAction(Action::Idle);
		break;
	}
}
