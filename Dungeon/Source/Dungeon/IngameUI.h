#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IngameUI.generated.h"





UCLASS()
class DUNGEON_API UIngameUI : public UUserWidget {
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UTextBlock* moneyText;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UImage*     moneyBackground;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UImage*     moneyIcon;
};
