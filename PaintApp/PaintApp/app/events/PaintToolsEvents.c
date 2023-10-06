#include "PaintToolsEvents.h"

DEFINE_OBJ_TYPE(PT_PropertiesUpdateEvent);
DEFINE_OBJ_TYPE(PT_ThemeUpdateEvent);

Event* PT_NewPropertiesUpdateEvent()
{
	return CreateCommonEvent(PT_PropertiesUpdateEvent);
}

Event* PT_NewThemeUpdateEvent()
{
	return CreateCommonEvent(PT_ThemeUpdateEvent);
}
