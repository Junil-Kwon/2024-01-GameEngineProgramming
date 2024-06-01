#include "XBow.h"
#include "Creature.h"

#include "GameFramework/CharacterMovementComponent.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AXBow::AXBow() {
	defaultWeaponRange = 0.0f;
}
void AXBow::BeginPlay() {
	Super::BeginPlay();
}

// =============================================================================================================
// Spawn
// =============================================================================================================

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

	FRotator rotation;
	FVector  location;
	switch (GetAction()) {
	case Action::Idle:
		if (parent == nullptr) {
			SetSpriteIndex(nullptr);
			SetSpriteAngle(nullptr);
		}
		else {
			rotation = ToRotator(parent->GetLookDirection());
			location = parent->GetActorLocation();
			location = location + ToVector(rotation) * (parent->GetHitboxRadius() * 0.5f);
			location += RotateVector(FVector(FMath::Abs(rotation.Yaw) < 60.0f ? 1.0f : -24.0f, 0.0f, 0.0f));
			SetActorLocation(location);
			SetSpriteIndex(nullptr, FMath::Min( 5 + int32(GetActionDelay() * 20),  7));
			SetSpriteXFlip(nullptr, false);
			SetSpriteAngle(nullptr, rotation.Yaw - 45.0f);
		}
		break;
	case Action::Attack:
		if (GetActionDelay() - DeltaTime == 0.0f) {
			// Spawn Arrow
			AEntity* arrow = Spawn(Identifier::Arrow, GetActorLocation() + ToVector(ToRotator(parent->GetLookDirection())) * 48.0f);
			arrow->SetGroup(GetGroup());
			arrow->SetSpriteAngle(nullptr, GetSpriteAngle());
			arrow->GetMovementComponent()->AddRadialImpulse(GetActorLocation(), 1024.0f, 1024.0f, ERadialImpulseFalloff::RIF_Linear, true);
			//
			parent->SetAction(Action::Idle);
			parent->SetActionCooldown(Action::Attack, 0.3f);
			parent->AddEffect(Effect::Slowness, 0.2f, 0.3f);
		}
		rotation = ToRotator(parent->GetLookDirection());
		location = parent->GetActorLocation();
		location = location + ToVector(rotation) * (parent->GetHitboxRadius() * 0.5f);
		location += RotateVector(FVector(FMath::Abs(rotation.Yaw) < 60.0f ? 1.0f : -24.0f, 0.0f, 0.0f));
		SetActorLocation(location);
		SetSpriteIndex(nullptr, FMath::Min(1 + int32(GetActionDelay() * 20), 4));
		SetSpriteXFlip(nullptr, false);
		SetSpriteAngle(nullptr, rotation.Yaw - 45.0f);
		if (0.3f <= GetActionDelay()) {
			SetAction(Action::Idle);
		}
		break;
	case Action::Defend:
		SetSpriteIndex(nullptr, FMath::Min( 8 + int32(GetActionDelay() * 20), 11));
		if (GetActionDelay() - DeltaTime == 0.0f) {
			rotation = ToRotator(parent->GetLookDirection());
			location = parent->GetActorLocation();
			location = location + ToVector(rotation) * (parent->GetHitboxRadius() * 0.5f);
			location += RotateVector(FVector(FMath::Abs(rotation.Yaw) < 60.0f ? 1.0f : -24.0f, 0.0f, 0.0f));
			SetActorLocation(location);
			SetSpriteXFlip(nullptr, false);
			SetSpriteAngle(nullptr, rotation.Yaw - 45.0f);
			parent->SetAction(Action::Idle);
			parent->SetActionCooldown(Action::Defend, 0.6f);
		}
		if (0.5f <= GetActionDelay()) SetAction(Action::Idle);
		break;
	}
}
