#pragma once

#include "JDefines.h"


#ifdef JACKC_APP_INTERFACE_HOME
#define JACKC_APP_INTERFACE_EXPORT DLL_EXPORT
#else 
#define JACKC_APP_INTERFACE_EXPORT DLL_IMPORT
#endif
