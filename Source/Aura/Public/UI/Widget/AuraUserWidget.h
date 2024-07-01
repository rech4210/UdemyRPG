// all rights reversed GGWA

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//(멀티캐스트 델리게이트 키워드)블루프린트 호출가능 - 이 프로퍼티는 블루프린트 그래프에서 호출 가능하도록 노출됩니다.
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
protected:
	//네이티브 함수가 블루프린트로 통신을 보낼 수 있도록 하는 주요한 방법입니다. 블루프린트 자체 내에 구현하는 가상 함수같은 것입니다.
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet(); //블루프린트에서 구현
	
};
