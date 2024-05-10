#pragma once

#include "CoreMinimal.h"
#include "Creature.h"
#include "Hero.generated.h"



UENUM(BlueprintType)
enum class Input : uint8 {
	Up				,
	Down			,
	Left			,
	Right			,
	Jump			,
	Dodge			,
	Act				,
	Swap			,
	Menu			,
	Max				UMETA(Hidden),
};

UENUM(BlueprintType)
enum class Sprite : uint8 {
	Idle			,
	Move			,
	Jump0			,
	Jump1			,
	Jump2			,
	Dodge0			,
	Dodge1			,
	Defeat			,
	Max				UMETA(Hidden),
};



UCLASS()
class FINALPROJECT_API AHero : public ACreature {
	GENERATED_BODY()
	
public:
	AHero();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	bool input[(uint8)Input::Max] = { false, };
	FVector direction = FVector::ZeroVector;
	void Up   (bool pressed);
	void Down (bool pressed);
	void Left (bool pressed);
	void Right(bool pressed);
	void Jump (bool pressed);
	void Dodge(bool pressed);
	void Act  (bool pressed);
	void Swap (bool pressed);
	void Menu (bool pressed);
	void UpdateInput(float DeltaTime);

public:
	UFUNCTION() void OnBeginSensed(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnEndSensed(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



protected:
	UPROPERTY(EditAnywhere) class USphereComponent* sensorComponent;
	UPROPERTY(VisibleAnywhere) class USpringArmComponent* springArmComponent;
	UPROPERTY(VisibleAnywhere) class UCameraComponent* cameraComponent;

protected:
	TArray<ACreature*> sensed;

protected:
	Sprite sprite     = Sprite::Idle;
	Sprite spritePrev = Sprite::Idle;
	virtual void UpdateSprite() override;

	virtual void Die() override;
};
