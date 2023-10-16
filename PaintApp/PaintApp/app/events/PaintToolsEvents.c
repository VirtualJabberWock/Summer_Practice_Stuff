#include "PaintToolsEvents.h"

DEFINE_OBJECT_TYPE_ONLY(PT_PropertiesUpdateEvent);
DEFINE_OBJECT_TYPE_ONLY(PT_ThemeUpdateEvent);

Event* PT_NewPropertiesUpdateEvent()
{
	return CreateCommonEvent(PT_PropertiesUpdateEvent);
}

Event* PT_NewThemeUpdateEvent()
{
	return CreateCommonEvent(PT_ThemeUpdateEvent);
}


