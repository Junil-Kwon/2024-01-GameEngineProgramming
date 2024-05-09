#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Indicator.generated.h"



FVector HexToColor(FString HexString);



UCLASS()
class FINALPROJECT_API AIndicator : public AActor {
	GENERATED_BODY()
	
public:
	AIndicator();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere) class UBoxComponent* boxComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* mesh0Component;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* mesh1Component;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* mesh2Component;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* mesh3Component;

private:
	float size = 16.0f;
};
