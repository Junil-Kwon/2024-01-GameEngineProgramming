#pragma once

#include "Entity.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ghost.generated.h"





UCLASS(HideCategories = ("Actor Tick", Camera, Replication, Rendering, Collision, Pawn, Input, LOD, Cooking))
class DUNGEON_API AGhost : public APawn {
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
	UPROPERTY(EditAnywhere) class USphereComponent*    sphereComponent;
	UPROPERTY(EditAnywhere) class USpringArmComponent* springComponent;
	UPROPERTY(EditAnywhere) class UCameraComponent*    cameraComponent;





	// Input
private:
	FVector direction = FVector::ZeroVector;
	bool inputDirection[4] = { false, };
	bool inputAction[(uint8)Action::Length] = { false, };
	void PUp    ();
	void RUp    ();
	void PDown  ();
	void RDown  ();
	void PLeft  ();
	void RLeft  ();
	void PRight ();
	void RRight ();
	void PJump  ();
	void RJump  ();
	void PDash  ();
	void RDash  ();
	void PAttack();
	void RAttack();
	void PDefend();
	void RDefend();
	void PMenu  ();
	void RMenu  ();
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	bool GetInput(Action value);
	FVector GetInputDirection();





	// Entity
private:
	TArray<AEntity*> objectPool[static_cast<uint8>(Identifier::Length)];
	UPROPERTY() class AEntity* player;
public:
	TArray<AEntity*>* GetObjectPool(Identifier value);
	AEntity* GetPlayer();
	void SetPlayer(AEntity* value);

	void OnPlayerSpawned  ();
	void OnPlayerDestroyed();
};
