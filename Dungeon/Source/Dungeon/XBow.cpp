#include "XBow.h"
#include "Creature.h"

#include "GameFramework/CharacterMovementComponent.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AXBow::AXBow() {
	defaultWeaponRange = 0.0f;
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

	float angle;
	FRotator rotation;
	FVector  location;
	switch (GetAction()) {
	case Action::Idle:
		if (parent == nullptr) {
			SetSpriteIndex(nullptr);
			SetSpriteAngle(nullptr);
		}
		else {


			/*
			rotation = ToRotator(parent->GetLookDirection());
			location = parent->GetActorLocation();
			location = location + ToVector(rotation) * (parent->GetHitboxRadius() * 0.5f);
			location += RotateVector(FVector(FMath::Abs(rotation.Yaw) < 60.0f ? 1.0f : -24.0f, 0.0f, 0.0f));
			SetActorLocation(location);
			SetSpriteIndex(nullptr, FMath::Min( 5 + int32(GetActionDelay() * 20),  7));
			SetSpriteXFlip(nullptr, false);
			SetSpriteAngle(nullptr, rotation.Yaw - 45.0f);
			*/
			angle = ToFloat(parent->GetLookDirection());
			location = parent->GetActorLocation();
			UE_LOG(LogTemp, Log, TEXT("%f"), angle);
			location += RotateVector(FVector(FMath::Abs(angle) < 60.0f ? 1.0f : -24.0f, 0.0f, 0.0f));
			SetActorLocation(location);
			SetSpriteIndex(nullptr, FMath::Min( 5 + int32(GetActionDelay() * 20),  7));
			SetSpriteXFlip(nullptr, false);
			SetSpriteAngle(nullptr, angle - 45.0f);
		}
		break;
	case Action::Attack:
		if (GetActionDelay() - DeltaTime == 0.0f) {
			AEntity* arrow = Spawn(Identifier::Arrow, parent->GetActorLocation());
			arrow->SetGroup(GetGroup());
			arrow->SetSpriteAngle(nullptr, GetSpriteAngle());
			arrow->LaunchCharacter(parent->GetLookDirection() * 4096.0f, true, true);
			//UE_LOG(LogTemp, Warning, TEXT("%f\t%f\t%f"), parent->GetLookDirection().X, parent->GetLookDirection().Y, parent->GetLookDirection().Z);
			/*
			AEntity* arrow = Spawn(Identifier::Arrow, GetActorLocation() + ToVector(ToRotator(parent->GetLookDirection())) * 48.0f);
			arrow->SetGroup(GetGroup());
			arrow->SetSpriteAngle(nullptr, GetSpriteAngle());
			arrow->LaunchCharacter(parent->GetLookDirection() * 4096.0f, true, true);
			*/
			if (parent->SetAction(parent->GetSprite())) parent->SetActionDelay(parent->GetSpriteDelay());
			parent->SetActionCooldown(Action::Attack, 0.3f);
			parent->AddEffect(Effect::Slowness, 0.2f, 0.3f);
		}
		angle = ToFloat(parent->GetLookDirection());
		location = parent->GetActorLocation();
		location += RotateVector(FVector(FMath::Abs(angle) < 60.0f ? 1.0f : -24.0f, 0.0f, 0.0f));
		SetActorLocation(location);
		SetSpriteIndex(nullptr, FMath::Min( 1 + int32(GetActionDelay() * 20), 4));
		SetSpriteXFlip(nullptr, false);
		SetSpriteAngle(nullptr, angle - 45.0f);

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
			if (parent->SetAction(parent->GetSprite())) parent->SetActionDelay(parent->GetSpriteDelay());
			parent->SetActionCooldown(Action::Defend, 0.6f);
		}
		if (0.5f <= GetActionDelay()) SetAction(Action::Idle);
		break;
	}
}
