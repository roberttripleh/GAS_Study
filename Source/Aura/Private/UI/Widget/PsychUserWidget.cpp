// Copyright Psych Ward 


#include "UI/Widget/PsychUserWidget.h"

void UPsychUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
