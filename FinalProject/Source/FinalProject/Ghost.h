#pragma once

#include "Creature.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ghost.generated.h"



UCLASS(HideCategories = ("Actor Tick", Camera, Replication, Rendering, Collision, Actor, Input, LOD, Cooking))
class FINALPROJECT_API AGhost : public APawn {
	GENERATED_BODY()

public:
	AGhost();
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;



	// Root
private:
	UPROPERTY() class USphereComponent* sphereComponent;

	// Camera
private:
	UPROPERTY() class USpringArmComponent* springArmComponent;
	UPROPERTY() class UCameraComponent* cameraComponent;

	// World
	#define DefaultWorldSpeed 1.0f
private:
	float worldSpeed = 1.0f;
public:
	float GetWorldSpeed();
	void  SetWorldSpeed(float value);

	// Player
private:
	UPROPERTY() class ACreature* player;
public:
	ACreature* GetPlayer();
	void SetPlayer(ACreature* value);


	// Input
	#define Up    0
	#define Down  1
	#define Left  2
	#define Right 3
private:
	FVector direction = FVector::ZeroVector;
	bool inputDirection[4] = { false, };
	bool inputAction[(uint8)Action::Length] = { false, };
	void InputMoveUp   (bool pressed);
	void InputMoveDown (bool pressed);
	void InputMoveLeft (bool pressed);
	void InputMoveRight(bool pressed);
	void InputJump     (bool pressed);
	void InputDash     (bool pressed);
	void InputAttack   (bool pressed);
	void InputDefend   (bool pressed);
	void InputMenu     (bool pressed);
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	bool GetInput(Action value);
	FVector GetInputDirection();
};
