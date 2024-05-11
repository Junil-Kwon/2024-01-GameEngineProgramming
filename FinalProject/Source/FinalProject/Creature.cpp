#include "Creature.h"
#include "Indicator.h"
#include "Particle.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"



ACreature::ACreature() {
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Creature"));
	GetCapsuleComponent()->InitCapsuleSize(24.0f, 50.0f);

	sensorComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SensorComponent"));
	sensorComponent->SetCollisionProfileName(TEXT("Sensor"));
	sensorComponent->InitSphereRadius(200.0f);
	sensorComponent->SetupAttachment(RootComponent);
}



void ACreature::BeginPlay() {
	Super::BeginPlay();

	sensorComponent->OnComponentBeginOverlap.AddDynamic(this, &ACreature::OnBeginSensed);
	sensorComponent->OnComponentEndOverlap  .AddDynamic(this, &ACreature::OnEndSensed);
	sensorRange = sensorComponent->GetScaledSphereRadius();

	indicator = static_cast<AIndicator*>(Spawn(Identifier::Indicator, FVector::ZeroVector));
	indicator->SetWidth(indicatorWidth);
	indicator->SetGroup(GetGroup());
	indicator->SetActorRelativeLocation(FVector(0.0f, 0.0f, GetHitboxHeight() * 0.5f + 80.0f));
	indicator->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	health = healthMax;
}



bool ACreature::operator==(const ACreature& other) const { return this == &other; }

void ACreature::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	UpdateSensor(DeltaTime);
}



float ACreature::GetSensorRange() {
	return sensorRange;
}
void  ACreature::SetSensorRange(float value) {
	sensorRange = value;
	sensorComponent->SetSphereRadius(sensorRange);
}
void ACreature::Select(ACreature* creature) {
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



AIndicator* ACreature::GetIndicator() { return indicator; }



float ACreature::GetHealth() { return health; }
void  ACreature::SetHealth(float value) { health = value; indicator->SetRatio(health / healthMax); }
void  ACreature::AdjustHealth(float value) {
	health = health + value < 0 ? 0 : (healthMax < health + value ? healthMax : health + value);
	if (health == 0) Die();
}
void ACreature::Die() {
	indicator->Destroy();
	Destroy();
}
