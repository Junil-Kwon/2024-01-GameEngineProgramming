#include "Creature.h"
#include "Indicator.h"



ACreature::ACreature() {
	effectStrength.Init(0.0f, static_cast<int32>(Effect::Max));
	effectDuration.Init(0.0f, static_cast<int32>(Effect::Max));

	health = healthMax;

	static ConstructorHelpers::FClassFinder<AIndicator> IndicatorClass(TEXT("/Game/Blueprints/BP_Indicator.BP_Indicator_C"));
	if (IndicatorClass.Succeeded()) indicator = IndicatorClass.Class;
}

void ACreature::BeginPlay() {
	Super::BeginPlay();

	if (indicator != nullptr) {
		AIndicator* Indicator = GetWorld()->SpawnActor<AIndicator>(indicator, FVector::ZeroVector, FRotator::ZeroRotator);
		if (Indicator != nullptr) {
			Indicator->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			Indicator->SetActorRelativeLocation(FVector(0.0f, 0.0f, 128.0f));
		}
	}
}

void ACreature::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}



void ACreature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}



float ACreature::GetHealth() { return health; }
void  ACreature::SetHealth(float value) { health = value; }
void  ACreature::AdjustHealth(float value) {
	health = health + value < 0 ? 0 : (healthMax < health + value ? healthMax : health + value);
	if (health == 0) Die();
}

void ACreature::Die() {
}
