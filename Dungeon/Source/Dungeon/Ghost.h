#pragma once

#include "Entity.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ghost.generated.h"





UCLASS(HideCategories = ("Actor Tick", Camera, Replication, Rendering, Collision, Pawn, Input, LOD, Cooking))
class DUNGEON_API AGhost : public APawn {
	GENERATED_BODY()





	// -
public:
	UTexture2D* GetTexture(FString name);
	UClass*     GetWidget (FString name);





	// Initialization
public:
	AGhost();
protected:
	virtual void BeginPlay() override;

	// Update
public:
	virtual void Tick(float DeltaTime) override;





	// Camera
	#define CameraSpeed 600.0f
private:
	class USphereComponent*    sphereComponent;
	class USpringArmComponent* springComponent;
	class UCameraComponent*    cameraComponent;
	bool     focusing;
	FVector  focusLocation;
	AEntity* focusEntity;
	bool  shakeVertical;
	float shakeStrength;
	float shakeDuration;
protected:
	void UpdateCamera(float DeltaTime);
public:
	void FocusCameraOn(AEntity* entity  );
	void FocusCameraOn(FVector  location);
	void UnfocusCamera();
	void ShakeCamera(float strength = 4.0f, float duration = 0.25f, bool vertical = true);





	// Money
private:
	int32 money;
public:
	int32 GetMoney();
	void  AdjustMoney(int32 value);

	// =========================================================================================================
	// UI
	// =========================================================================================================

private:
	class UIngameUI* ingameUI;

	// Money
private:
	float moneyTemp;
	int32 moneyIcon;
	int32 moneySize;
	float moneyXPos;
	float moneyOpacity;
	float moneyDuration;
protected:
	void UpdateMoney(float DeltaTime);

	// Keyboard
	#define KeyboardShowDuration 30.0f
private:
	bool  keyboardUp;
	bool  keyboardDown;
	bool  keyboardLeft;
	bool  keyboardRight;
	bool  keyboardSpace;
	bool  keyboardShift;
	bool  keyboardZ;
	bool  keyboardX;
	bool  keyboardC;
	float keyboardOpacity;
	float keyboardDuration;
protected:
	void UpdateKeyboard(float DeltaTime);





	// Entity
private:
	TArray<class AEntity*> objectPool[static_cast<uint8>(Identifier::Length)];
	class AEntity* player;
public:
	TArray<class AEntity*>* GetObjectPool(Identifier value);
	AEntity* GetPlayer();
	void     SetPlayer(AEntity* value);

	void OnPlayerSpawned();
	void OnPlayerDestroyed();





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
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	bool GetInput(Action value);
	FVector GetInputDirection();
};
