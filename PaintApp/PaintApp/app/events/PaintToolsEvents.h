#pragma once

#include "../core/messaging/Event.h"

typedef Event PT_PropertiesUpdateEvent;
Event* PT_NewPropertiesUpdateEvent();

typedef Event PT_ThemeUpdateEvent;
Event* PT_NewThemeUpdateEvent();