// all rights reversed GGWA


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController) {

	WidgetController = InWidgetController;
	
	WidgetControllerSet();
}
