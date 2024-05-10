#include "Particle.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"



AParticle::AParticle() {
	PrimaryActorTick.bCanEverTick = true;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	boxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(boxComponent);

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh0Component"));
	meshComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 41.409618f));
	meshComponent->SetRelativeScale3D(FVector(1.28f, 1.28f, 1.28f));
	meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	meshComponent->SetupAttachment(boxComponent);
}

void AParticle::BeginPlay() {
	Super::BeginPlay();
}

void AParticle::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	UpdateSprite(DeltaTime);
}



void AParticle::UpdateSprite(float DeltaTime) {
	spriteDelay += DeltaTime;
	switch (identifier) {
	case Identifier::Dust: if (0.5f < spriteDelay) Destroy(); break;
	}

	int32 i = 0;
	switch (identifier) {
		case Identifier::Dust: i = 1 + static_cast<int32>(spriteDelay * 10); if (5 < i) i = 5; break;
	}
	if (spriteIndex == i) return;
	spriteIndex = i;
	meshComponent->SetScalarParameterValueOnMaterials(TEXT("Index"), spriteIndex);
	//meshComponent->SetScalarParameterValueOnMaterials(TEXT("XFlip"), spriteXflip ? 1.0f : 0.0f);
}