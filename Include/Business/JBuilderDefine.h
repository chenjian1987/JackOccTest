#pragma once
#include "JackDefines.h"

#ifdef JACK_BUILDER_DEFINE_HOME
#define JACK_BUILDER_DEFINE_EXPORT DLL_EXPORT
#else
#define JACK_BUILDER_DEFINE_EXPORT DLL_IMPORT
#endif