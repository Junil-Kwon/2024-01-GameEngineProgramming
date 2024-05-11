#include "Indicator.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"



AIndicator::AIndicator() {
 	PrimaryActorTick.bCanEverTick = true;

	sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	sphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(sphereComponent);

	sprite0Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh0Component"));
	sprite0Component->SetWorldLocation(FVector(0.0f, -64.0f - 2.0f, 0.0f));
	sprite0Component->SetWorldRotation(FRotator(0.0f, 90.0f, 41.409618f));
	sprite0Component->SetWorldScale3D(FVector(0.64f, 1.28f, 1.28f));
	sprite0Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	sprite0Component->SetupAttachment(sphereComponent);

	sprite1Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh1Component"));
	sprite1Component->SetWorldLocation(FVector(0.0f,  64.0f + 2.0f, 0.0f));
	sprite1Component->SetWorldRotation(FRotator(0.0f, 90.0f, 41.409618f));
	sprite1Component->SetWorldScale3D(FVector(0.64f, 1.28f, 1.28f));
	sprite1Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	sprite1Component->SetupAttachment(sphereComponent);

	sprite2Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh2Component"));
	sprite2Component->SetWorldRotation(FRotator(0.0f, 90.0f, 41.409618f));
	sprite2Component->SetWorldScale3D(FVector(1.28f, 1.28f, 1.28f));
	sprite2Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	sprite2Component->SetupAttachment(sphereComponent);

	sprite3Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh3Component"));
	sprite3Component->SetWorldLocation(FVector(-0.5f, 0.0f, 0.5f));
	sprite3Component->SetWorldRotation(FRotator(0.0f, 90.0f, 41.409618f));
	sprite3Component->SetWorldScale3D(FVector(1.28f, 1.28f, 1.28f));
	sprite3Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	sprite3Component->SetupAttachment(sphereComponent);
}



void AIndicator::BeginPlay() {
	Super::BeginPlay();
	
	SetWidth(16);
	SetRatio( 1);
	sprite0Component->SetScalarParameterValueOnMaterials(TEXT("Index"), 0);
	sprite1Component->SetScalarParameterValueOnMaterials(TEXT("Index"), 0);
	sprite2Component->SetScalarParameterValueOnMaterials(TEXT("Index"), 4);
	sprite3Component->SetScalarParameterValueOnMaterials(TEXT("Index"), 8);
	SetGroup(Group::None);
}



void AIndicator::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}



void AIndicator::SetWidth(float value) {
	width = value;
	sprite0Component->SetRelativeLocation(FVector(0.0f, width * -2.0f - 2.0f, 0.0f));
	sprite1Component->SetRelativeLocation(FVector(0.0f, width *  2.0f + 2.0f, 0.0f));
	sprite2Component->SetRelativeScale3D(FVector(width * 0.04f, 1.28f, 1.28f));
	sprite3Component->SetRelativeScale3D(FVector(width * 0.04f, 1.28f, 1.28f));
}

void AIndicator::SetRatio(float value) {
	sprite3Component->SetRelativeLocation(FVector(-0.5f, width * (1 - value) * -2.0f, 0.5f));
	sprite3Component->SetRelativeScale3D(FVector(width * value * 0.04f, 1.28f, 1.28f));
}

void AIndicator::SetGroup(Group value) {
	FVector color = FVector::OneVector;
	switch (value) {
	case Group::Friendly: color = FVector(0.031896, 0.332452, 0.152926); break;
	case Group::Enemy:    color = FVector(0.332452, 0.044270, 0.064128); break;
	}
	sprite3Component->SetVectorParameterValueOnMaterials(TEXT("Color"), color);
}