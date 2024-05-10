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



protected:
	UPROPERTY(EditAnywhere) class UBoxComponent* boxComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* mesh0Component;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* mesh1Component;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* mesh2Component;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* mesh3Component;

protected:
	float width = 24.0f;
public:
	void SetWidth(float value);
	void SetRatio(float value);
	void SetGroup(Group value);
};
