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
	
	mesh0Component->SetScalarParameterValueOnMaterials(TEXT("Index"), 0);
	mesh1Component->SetScalarParameterValueOnMaterials(TEXT("Index"), 0);
	mesh2Component->SetScalarParameterValueOnMaterials(TEXT("Index"), 4);
	mesh3Component->SetScalarParameterValueOnMaterials(TEXT("Index"), 8);

	mesh0Component->SetRelativeLocation(FVector(0.0f, size * -2.0f - 2.0f, 0.0f));
	mesh1Component->SetRelativeLocation(FVector(0.0f, size *  2.0f + 2.0f, 0.0f));
	mesh2Component->SetRelativeScale3D(FVector(size * 0.04f, 1.28f, 1.28f));
	mesh3Component->SetRelativeScale3D(FVector(size * 0.04f, 1.28f, 1.28f));
}

FVector HexToColor(FString HexString) {
	FColor color = FColor::FromHex(HexString);
	UE_LOG(LogTemp, Log, TEXT("%f\t%f\t%f"), color.R / 255.0f, color.G / 255.0f, color.B / 255.0f);
	return FVector(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f);
}

void AIndicator::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

