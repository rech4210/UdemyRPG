// all rights reversed GGWA


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
//thanks rider makes include!
#include "UI/WidgetController/OverlayWidgetController.h"


UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams) {
	// if already generated, store pointer.
	if(OverlayWidgetController == nullptr) {
		//private 멤버 controller를 여기서 할당해줌.
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		//FWidgetControllerParams have abilitySystem, attribute set... struct
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

// first set FWidgetControllerParams, then store pointer UOverlayWidgetController
// HUD에서 model의 Params값을 가리키는 pointer를 가져오고 이 값을 가질 controller를 생성하고 params를 인자로 넣어준다음.
// OverlayWidget->SetWidgetController(OverlayWidgetController); 여기서 widget이 controller를 가리키도록 한다.
void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS) {
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class unintialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD"))
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);
	
	const FWidgetControllerParams WidgetControllerParams(PC,PS , ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	
	// now widget know which is controller
	OverlayWidget->SetWidgetController(WidgetController);
	//위젯이 controller에 부착되고 난 후, controller는 Init으로 델리게이트 초기화를 한다.
	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();
}

