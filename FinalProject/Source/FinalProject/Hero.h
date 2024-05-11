#pragma once

#include "CoreMinimal.h"
#include "Creature.h"
#include "Hero.generated.h"



UENUM(BlueprintType)
enum class Input : uint8 {
	Up				= 0,
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
	Idle			= 0,
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



	// Sprite
protected:
	Sprite sprite     = Sprite::Idle;
	Sprite spritePrev = Sprite::Idle;
	virtual void UpdateSprite(float DeltaTime) override;

	// Sensor
protected:
	virtual void Select(ACreature* creature) override;
	virtual void UpdateSensor(float DeltaTime) override;
public:
	virtual void OnBeginSensed(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndSensed(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	// Stats
protected:
	virtual void Die() override;

	// Camera
protected:
	UPROPERTY(VisibleAnywhere) class USpringArmComponent* springArmComponent;
	UPROPERTY(VisibleAnywhere) class UCameraComponent* cameraComponent;

	// Input
private:
	bool input[(uint8)Input::Max] = { false, };
	void Up   (bool pressed);
	void Down (bool pressed);
	void Left (bool pressed);
	void Right(bool pressed);
	void Jump (bool pressed);
	void Dodge(bool pressed);
	void Act  (bool pressed);
	void Swap (bool pressed);
	void Menu (bool pressed);
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	void UpdateInput(float DeltaTime);
};
