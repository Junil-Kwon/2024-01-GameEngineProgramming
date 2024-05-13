#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ghost.generated.h"



UENUM(BlueprintType)
enum class Input : uint8 {
	Up				= 0,
	Down			,
	Left			,
	Right			,
	Space			,
	Shift			,
	Z				,
	X				,
	C				,
	Max				UMETA(Hidden),
};



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

	// Input
private:
	bool input[(uint8)Input::Max] = { false, };
	FVector direction = FVector::ZeroVector;
	void Up   (bool pressed);
	void Down (bool pressed);
	void Left (bool pressed);
	void Right(bool pressed);
	void Space(bool pressed);
	void Shift(bool pressed);
	void Z    (bool pressed);
	void X    (bool pressed);
	void C    (bool pressed);
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	bool* GetInput();
	FVector GetInputDirection();
};
