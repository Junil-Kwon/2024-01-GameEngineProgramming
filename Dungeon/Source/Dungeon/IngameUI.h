#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IngameUI.generated.h"





UCLASS()
class DUNGEON_API UIngameUI : public UUserWidget {
	GENERATED_BODY()
	




public:
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UTextBlock* moneyText;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UImage*     moneyIcon;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UImage*     moneyBackground;

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UImage* keyboardShift;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UImage* keyboardZ;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UImage* keyboardX;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UImage* keyboardC;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UImage* keyboardSpace;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UImage* keyboardUp;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UImage* keyboardDown;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UImage* keyboardLeft;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UImage* keyboardRight;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UImage* keyboardBackground;
};
