#pragma once

#ifdef _MSC_VER
#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)
#else
#define DLL_EXPORT
#define DLL_IMPORT
#endif


#ifdef JACKC_APP_INTERFACE_TEST_HOME
#define JACKC_APP_INTERFACE_TEST_EXPORT DLL_EXPORT
#else 
#define JACKC_APP_INTERFACE_TEST_EXPORT DLL_IMPORT;
#endif




