#pragma once

#include "Entity.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ghost.generated.h"





UCLASS(HideCategories = ("Actor Tick", Camera, Replication, Rendering, Collision, Pawn, Input, LOD, Cooking))
class FINALPROJECT_API AGhost : public APawn {
	GENERATED_BODY()





	// Initialization
public:
	AGhost();
protected:
	virtual void BeginPlay() override;

	// Update
public:
	virtual void Tick(float DeltaTime) override;





	// Camera
private:
	UPROPERTY() class USphereComponent*    sphereComponent;
	UPROPERTY() class USpringArmComponent* springComponent;
	UPROPERTY() class UCameraComponent*    cameraComponent;





	// Input
private:
	FVector direction = FVector::ZeroVector;
	bool inputDirection[4] = { false, };
	bool inputAction[(uint8)Action::Length] = { false, };
	void MoveUp   (bool pressed);
	void MoveDown (bool pressed);
	void MoveLeft (bool pressed);
	void MoveRight(bool pressed);
	void Jump     (bool pressed);
	void Dash     (bool pressed);
	void Attack   (bool pressed);
	void Defend   (bool pressed);
	void Menu     (bool pressed);
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	bool GetInput(Action value);
	FVector GetInputDirection();





	// Player
private:
	UPROPERTY() class AEntity* player;
public:
	AEntity* GetPlayer();
	void SetPlayer(AEntity* value);
};
