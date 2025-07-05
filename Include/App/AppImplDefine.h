#pragma once

#include "JDefines.h"

#ifdef JACKC_APP_IMPL_HOME
#define JACKC_APP_IMPL_EXPORT DLL_EXPORT
#else 
#define JACKC_APP_IMPL_EXPORT DLL_IMPORT
#endif

