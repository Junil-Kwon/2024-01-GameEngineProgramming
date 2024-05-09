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
	Action			,
	Swap			,
	Menu			,
	Max				UMETA(Hidden),
};

UENUM(BlueprintType)
enum class Sprite : uint8 {
	Idle			,
	Move			,
	Jump			,
	Dodge			,
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
	void Up    (bool pressed);
	void Down  (bool pressed);
	void Left  (bool pressed);
	void Right (bool pressed);
	void Jump  (bool pressed);
	void Dodge (bool pressed);
	void Action(bool pressed);
	void Swap  (bool pressed);
	void Menu  (bool pressed);

protected:
	UPROPERTY(VisibleAnywhere) class USpringArmComponent* springArmComponent;
	UPROPERTY(VisibleAnywhere) class UCameraComponent* cameraComponent;

	Sprite sprite = Sprite::Idle, spritePrev = Sprite::Idle;
	int32 index = 0;
	float delay = 0.0f;
	bool  xflip = false;

	void UpdateSprite(float DeltaTime);

	bool input[(uint8)Input::Max] = { false, };
	FVector direction = FVector::ZeroVector;
};
