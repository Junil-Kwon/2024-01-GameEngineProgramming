#include "Creature.h"
#include "Ghost.h"
#include "Indicator.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"



ACreature::ACreature() {
	SetHitbox(36.0f, 100.0f);
	SetCollisionProfileName("Creature");

	sensorComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sensor"));
	sensorComponent->InitSphereRadius(DefaultSensorRange);
	sensorComponent->SetCollisionProfileName("Sensor");
	sensorComponent->SetupAttachment(RootComponent);

	magnetComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Magnet"));
	magnetComponent->InitSphereRadius(DefaultMagnetRange);
	magnetComponent->SetCollisionProfileName("Sensor");
	magnetComponent->SetupAttachment(RootComponent);
}



void ACreature::BeginPlay() {
	Super::BeginPlay();

	SetSensorRange(sensorRange);
	sensorComponent->OnComponentBeginOverlap.AddDynamic(this, &ACreature::OnSensorBeginOverlap);
	sensorComponent->OnComponentEndOverlap  .AddDynamic(this, &ACreature::OnSensorEndOverlap  );

	SetMagnetRange(magnetRange);
	magnetComponent->OnComponentBeginOverlap.AddDynamic(this, &ACreature::OnMagnetBeginOverlap);
	magnetComponent->OnComponentEndOverlap  .AddDynamic(this, &ACreature::OnMagnetEndOverlap  );
	
	indicator = static_cast<AIndicator*>(Spawn(Identifier::Indicator));
	if (indicator) {
		indicator->SetWidth(indicatorWidth);
		indicator->SetGroup(GetGroup());
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		indicator->AttachToActor(this, AttachmentRules);
		indicator->SetActorRelativeLocation(FVector(0.0f, 0.0f, GetHitboxHeight() * 0.5f + 80.0f));
	}
	
	SetGroup(GetGroup());

	healthMax = health;
	shieldMax = shield;
}



void ACreature::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (GetWorldSpeed() != 0.0f) UpdateSensor(DeltaTime * GetWorldSpeed());
	if (GetWorldSpeed() != 0.0f) UpdateMagnet(DeltaTime * GetWorldSpeed());
}



bool ACreature::UpdateAction(float DeltaTime) {
	if (!Super::UpdateAction(DeltaTime)) return false;

	if (0.0f < energeMax) {
		energe = FMath::Max(energe + energeMax * 0.1f * DeltaTime, energeMax);
	}
	if (0.0f < hurtCooldown) {
		if ((hurtCooldown -= DeltaTime) < 0.0f) {

		}
	}
	if (shield && shield != shieldMax) {
		if ((mendCooldown -= DeltaTime) < 0.0f) {
			shield = FMath::Max(shield + shieldMax * 0.1f, shieldMax);
			mendCooldown = MendCooldown;
		}
	}
	return true;
}



void ACreature::SetGroup(Group value) {
	Super::SetGroup(value);
	if (indicator) indicator->SetGroup(value);
}



bool ACreature::AddTag(Tag value) {
	if (!Super::AddTag(value)) return false;
	switch (value) {
	case Tag::Floating:        break;
	case Tag::Piercing:        break;
	case Tag::Invulnerability: break;
	case Tag::Interactability: break;
	case Tag::Collectable:     break;
	case Tag::Player:          GetGhost()->SetPlayer(this); break;
	case Tag::Leader:          break;
	}
	return true;
}
bool ACreature::RemoveTag(Tag value) {
	if (!Super::RemoveTag(value)) return false;
	switch (value) {
	case Tag::Floating:        break;
	case Tag::Piercing:        break;
	case Tag::Invulnerability: break;
	case Tag::Interactability: break;
	case Tag::Collectable:     break;
	case Tag::Player:          if (GetGhost()->GetPlayer() == this) GetGhost()->SetPlayer(nullptr); break;
	case Tag::Leader:          break;
	}
	return true;
}



bool ACreature::UpdateEffect(float DeltaTime) {
	if (!Super::UpdateEffect(DeltaTime)) return false;
	for (uint8 i = 0; i < static_cast<uint8>(Effect::Length); i++) {
		Effect index = static_cast<Effect>(1 << i);
		// color effect, tick damage, etc.
	}
	return true;
}
bool ACreature::AddEffect(Effect value, float strength, float duration) {
	if (!Super::AddEffect(value, strength, duration)) return false;
	switch (value) {
	case Effect::HealthBoost: break;
	case Effect::DamageBoost: break;
	case Effect::Resistance:  break;
	case Effect::Speed:       break;
	case Effect::Burn:        break;
	case Effect::Stun:        break;
	case Effect::Freeze:      break;
	}
	return true;
}
bool ACreature::RemoveEffect(Effect value) {
	if (!Super::RemoveEffect(value)) return false;
	switch (value) {
	case Effect::HealthBoost: break;
	case Effect::DamageBoost: break;
	case Effect::Resistance:  break;
	case Effect::Speed:       break;
	case Effect::Burn:        break;
	case Effect::Stun:        break;
	case Effect::Freeze:      break;
	}
	return true;
}



bool ACreature::GetInput(Action value) {
	if (!GetGhost()) return nullptr;
	return GetGhost()->GetInput(value);
}
FVector ACreature::GetInputDirection() {
	if (!GetGhost()) return FVector::ZeroVector;
	return GetGhost()->GetInputDirection();
}
bool ACreature::UpdateOutput() {
	if (HasTag(Tag::Player)) {
		switch (GetAction()) {
		case Action::Idle:
			 break;
		case Action::Move:
			 break;
		case Action::Jump:

			break;
		case Action::Dodge:
			 break;
		}
		return false;
	}
	return true;
}




float ACreature::GetSensorRange() {
	return sensorRange;
}
void  ACreature::SetSensorRange(float range) {
	sensorRange = range;
	sensorComponent->SetSphereRadius(range);
}
bool ACreature::UpdateSensor(float DeltaTime) {
	if (sensorArray.Num() == 0) return false;
	return true;
}
void ACreature::OnSensorBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult) {
}
void ACreature::OnSensorEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
}



float ACreature::GetMagnetRange() {
	return magnetRange;
}
void  ACreature::SetMagnetRange(float range) {
	magnetRange = range;
	magnetComponent->SetSphereRadius(range);
}
bool ACreature::UpdateMagnet(float DeltaTime) {
	if (magnetArray.Num() == 0) return false;
	if (GetGroup() != GetGhost()->GetPlayer()->GetGroup()) return false;

	for (int32 i = magnetArray.Num() - 1; -1 < i; i--) {
		if (magnetArray[i] == nullptr) {
			magnetArray.RemoveAt(i);
			continue;
		}
		// pull other actor to this actor
	}
	if (HasTag(Tag::Player)) {
		// player interaction
		AEntity* entity = nullptr;
		float distance = magnetRange;
		for (int32 i = 0; i < magnetArray.Num(); i++) {
			if (HasTag(Tag::Collectable)) continue;
			float s = FVector::Distance(magnetArray[i]->GetActorLocation(), GetActorLocation());
			if (s < distance) {
				entity = magnetArray[i];
				distance = s;
			}
		}
		Select(entity);
	}
	return true;
}
void ACreature::OnMagnetBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult) {

	AEntity* entity = Cast<AEntity>(OtherActor);
	if (entity == nullptr) return;
	if (!magnetArray.Contains(entity) && entity->HasTag(Tag::Interactability)) {
		magnetArray.Add(entity);
		if (selected == nullptr) Select(entity);
	}
}
void ACreature::OnMagnetEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	AEntity* entity = Cast<AEntity>(OtherActor);
	if (entity == nullptr) return;
	if (magnetArray.Contains(entity)) {
		magnetArray.Remove(entity);
		if (selected == entity) Select(nullptr);
	}
}



void ACreature::Select(AEntity* entity) {
	if (entity == selected) return;
	if (selected != nullptr) {
		// deselect 'selected'
		selected = nullptr;
	}
	if (entity != nullptr) {
		selected = entity;
		// select 'selected'
	}
}



AIndicator* ACreature::GetIndicator() {
	return indicator;
}



float ACreature::GetHealth() { return health + shield + GetEffectStrength(GetIndex(Effect::HealthBoost)); }
void  ACreature::OnDamaged(float value) {
	if (0.0f < value) {
		if (HasTag(Tag::Invulnerability)) return;
		if (HasEffect(Effect::Resistance)) value *= 1 - GetEffectStrength(GetIndex(Effect::Resistance));
		if (HasEffect(Effect::HealthBoost)) {
			uint8 i = GetIndex(Effect::HealthBoost);
			float absorption = FMath::Max(GetEffectStrength(i), value) - FMath::Min(GetEffectStrength(i), value);
			AdjustEffectStrength(i, -absorption);
			value -= absorption;
		}
		hurtCooldown = HurtCooldown;
		mendCooldown = MendCooldown;
	}
	float shieldTemp = shield;
	if (shield) shield = FMath::Clamp(shield - value, 0.0f, shieldMax);
	else        health = FMath::Clamp(health - value, 0.0f, healthMax);
	if (shield == 0.0f && shieldTemp) OnShieldBroken();
	if (health == 0.0f) OnDie();
	// refresh indicator
}
void  ACreature::OnShieldBroken() {
	shieldMax = 0.0f;
}
void  ACreature::OnDie() {
	SetAction(Action::Defeat);
	// if (HasEffect(Effect::Burn)) smoke effect, if enough strength, ashed effect
}



void ACreature::AdjustMaxHealth(float value) {
	healthMax += FMath::Min(healthMax + value, 0.0f);
	health    += FMath::Min(health    + value, 0.0f);
	if (health == 0.0f) OnDie();
	// refresh indicator
}
void ACreature::AdjustMaxShield(float value) {
	shieldMax += FMath::Min(shieldMax + value, 0.0f);
	shield    += FMath::Min(shield    + value, 0.0f);
	if (shield == 0.0f) OnShieldBroken();
	// refresh indicator
}
void ACreature::AdjustMaxEnerge(float value) {
	energeMax += FMath::Min(energeMax + value, 0.0f);
	energe    += FMath::Min(energe    + value, 0.0f);
	// refresh indicator
}
void ACreature::AdjustMaxDamage(float value) {
	damage = FMath::Min(damage + value, 0.0f);
}
