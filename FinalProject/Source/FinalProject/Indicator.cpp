#include "Indicator.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"



AIndicator::AIndicator() {
 	PrimaryActorTick.bCanEverTick = true;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	boxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(boxComponent);

	mesh0Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh0Component"));
	mesh0Component->SetRelativeLocation(FVector(0.0f, -64.0f - 2.0f, 0.0f));
	mesh0Component->SetRelativeRotation(FRotator(0.0f, 90.0f, 41.409618f));
	mesh0Component->SetRelativeScale3D(FVector(0.64f, 1.28f, 1.28f));
	mesh0Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mesh0Component->SetupAttachment(boxComponent);

	mesh1Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh1Component"));
	mesh1Component->SetRelativeLocation(FVector(0.0f,  64.0f + 2.0f, 0.0f));
	mesh1Component->SetRelativeRotation(FRotator(0.0f, 90.0f, 41.409618f));
	mesh1Component->SetRelativeScale3D(FVector(0.64f, 1.28f, 1.28f));
	mesh1Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mesh1Component->SetupAttachment(boxComponent);

	mesh2Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh2Component"));
	mesh2Component->SetRelativeRotation(FRotator(0.0f, 90.0f, 41.409618f));
	mesh2Component->SetRelativeScale3D(FVector(1.28f, 1.28f, 1.28f));
	mesh2Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mesh2Component->SetupAttachment(boxComponent);

	mesh3Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh3Component"));
	mesh3Component->SetRelativeLocation(FVector(-0.5f, 0.0f, 0.5f));
	mesh3Component->SetRelativeRotation(FRotator(0.0f, 90.0f, 41.409618f));
	mesh3Component->SetRelativeScale3D(FVector(1.28f, 1.28f, 1.28f));
	mesh3Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mesh3Component->SetupAttachment(boxComponent);
}

void AIndicator::BeginPlay() {
	Super::BeginPlay();
	
	SetWidth(16);
	SetRatio( 1);
	mesh0Component->SetScalarParameterValueOnMaterials(TEXT("Index"), 0);
	mesh1Component->SetScalarParameterValueOnMaterials(TEXT("Index"), 0);
	mesh2Component->SetScalarParameterValueOnMaterials(TEXT("Index"), 4);
	mesh3Component->SetScalarParameterValueOnMaterials(TEXT("Index"), 8);
	SetGroup(Group::None);
}

void AIndicator::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AIndicator::SetWidth(float value) {
	width = value;
	mesh0Component->SetRelativeLocation(FVector(0.0f, width * -2.0f - 2.0f, 0.0f));
	mesh1Component->SetRelativeLocation(FVector(0.0f, width *  2.0f + 2.0f, 0.0f));
	mesh2Component->SetRelativeScale3D(FVector(width * 0.04f, 1.28f, 1.28f));
	mesh3Component->SetRelativeScale3D(FVector(width * 0.04f, 1.28f, 1.28f));
}

void AIndicator::SetRatio(float value) {
	mesh3Component->SetRelativeLocation(FVector(-0.5f, width * (1 - value) * -2.0f, 0.5f));
	mesh3Component->SetRelativeScale3D(FVector(width * value * 0.04f, 1.28f, 1.28f));
}

void AIndicator::SetGroup(Group value) {
	FVector color = FVector::OneVector;
	switch (value) {
	case Group::Friendly: color = FVector(0.031896, 0.332452, 0.152926); break;
	case Group::Enemy:    color = FVector(0.332452, 0.044270, 0.064128); break;
	}
	mesh3Component->SetVectorParameterValueOnMaterials(TEXT("Color"), color);
}