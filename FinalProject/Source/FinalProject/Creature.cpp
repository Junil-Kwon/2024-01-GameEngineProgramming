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
	indicator->SetWidth(indicatorWidth);
	indicator->SetGroup(GetGroup());
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	indicator->AttachToActor(this, AttachmentRules);
	indicator->SetActorRelativeLocation(FVector(0.0f, 0.0f, GetHitboxHeight() * 0.5f + 80.0f));
	
	health = healthMax;
	shield = shieldMax;
}



void ACreature::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (GetWorldSpeed() != 0.0f) UpdateSensor(DeltaTime * GetWorldSpeed());
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
void ACreature::UpdateSensor(float DeltaTime) {
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
void ACreature::UpdateMagnet(float DeltaTime) {
}
void ACreature::OnMagnetBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult) {
}
void ACreature::OnMagnetEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
}



void ACreature::Select(AEntity* entity) {
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
