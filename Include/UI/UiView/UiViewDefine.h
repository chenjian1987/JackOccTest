#pragma once

#include "JDefines.h"


#ifdef JACKC_UIVIEW_HOME
#define JACKC_UIVIEW_EXPORT DLL_EXPORT
#else 
#define JACKC_UIVIEW_EXPORT DLL_IMPORT
#endif
