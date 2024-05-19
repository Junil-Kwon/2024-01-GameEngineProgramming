#include "Creature.h"
#include "Indicator.h"
#include "Components/CapsuleComponent.h"



ACreature::ACreature() {
	GetCapsuleComponent()->InitCapsuleSize(36.0f, 60.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Creature"));
	
	effectStrength.Init(0.0f, static_cast<int32>(Effect::Max));
	effectDuration.Init(0.0f, static_cast<int32>(Effect::Max));

	health = healthMax;
}

void ACreature::BeginPlay() {
	Super::BeginPlay();

	indicator = GetWorld()->SpawnActor<AIndicator>(indicatorClass, FVector::ZeroVector, FRotator::ZeroRotator);
	indicator->SetActorRelativeLocation(FVector(0.0f, 0.0f, 128.0f));
	indicator->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	indicator->SetWidth(indicatorWidth);
	indicator->SetGroup(group);
}

void ACreature::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

bool ACreature::operator==(const ACreature& other) const {
	return this == &other;
}



float ACreature::GetHealth() { return health; }
void  ACreature::SetHealth(float value) { health = value; indicator->SetRatio(health / healthMax); }
void  ACreature::AdjustHealth(float value) {
	health = health + value < 0 ? 0 : (healthMax < health + value ? healthMax : health + value);
	if (health == 0) Die();
}
void ACreature::Die() {
}
