#pragma once

#include "Entity.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Indicator.generated.h"



UCLASS()
class FINALPROJECT_API AIndicator : public AActor {
	GENERATED_BODY()
	
public:
	AIndicator();
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;



	// Identifier
private:
	UPROPERTY(VisibleAnywhere) Identifier identifier = Identifier::Indicator;

	// Sprite
private:
	float width = 24.0f;
protected:
	UPROPERTY(EditAnywhere) class USphereComponent* sphereComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* sprite0Component;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* sprite1Component;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* sprite2Component;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* sprite3Component;
public:
	void SetWidth(float value);
	void SetRatio(float value);
	void SetGroup(Group value);
};