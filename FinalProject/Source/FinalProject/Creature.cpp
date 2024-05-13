#include "Creature.h"
#include "Ghost.h"
#include "Indicator.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"



ACreature::ACreature() {
	SetHitbox(0.36f, 1.00f);
	SetCollisionProfileName("Creature");

	sensorComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sensor"));
	sensorComponent->InitSphereRadius(0.0f);
	sensorComponent->SetCollisionProfileName("Sensor");
	sensorComponent->SetupAttachment(RootComponent);
}



void ACreature::BeginPlay() {
	Super::BeginPlay();

	SetSensorRange(sensorRange);
	sensorComponent->OnComponentBeginOverlap.AddDynamic(this, &ACreature::OnBeginSensed);
	sensorComponent->OnComponentEndOverlap  .AddDynamic(this, &ACreature::OnEndSensed  );
	
	indicator = static_cast<AIndicator*>(Spawn(Identifier::Indicator));
	indicator->SetWidth(indicatorWidth);
	indicator->SetGroup(GetGroup());
	indicator->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	indicator->SetActorRelativeLocation(FVector(0.0f, 0.0f, GetHitboxHeight() * 0.5f + 80.0f));

	health = healthMax;
	shield = shieldMax;
}



void ACreature::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	UpdateSensor(DeltaTime);
}



bool* ACreature::GetInput() {
	static AGhost* ghost = nullptr;
	if (ghost == nullptr) ghost = static_cast<AGhost*>(UGameplayStatics::GetPlayerPawn(this, 0));
	return ghost->GetInput();
}
FVector ACreature::GetInputDirection() {
	static AGhost* ghost = nullptr;
	if (ghost == nullptr) ghost = static_cast<AGhost*>(UGameplayStatics::GetPlayerPawn(this, 0));
	return ghost->GetInputDirection();
}



float ACreature::GetSensorRange() {
	return sensorRange;
}
void  ACreature::SetSensorRange(float range) {
	sensorRange = range;
	sensorComponent->SetSphereRadius(range);
}
void ACreature::Select(AEntity* entity) {
}
void ACreature::UpdateSensor(float DeltaTime) {
}
void ACreature::OnBeginSensed(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* PtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
}
void ACreature::OnEndSensed(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
}



AIndicator* ACreature::GetIndicator() {
	return indicator;
}



float ACreature::GetHealth() { return health + shield + GetEffectStrength(GetIndex(Effect::HealthBoost)); }
void  ACreature::AdjustHealth(float value) {
	// shield - health - boost
}
void  ACreature::OnDie() {
}
