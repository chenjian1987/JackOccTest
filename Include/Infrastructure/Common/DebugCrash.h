#pragma once

#include "CrashHelper.h"

//__FILE__用以指示本行语句所在源文件的文件名
#ifdef _WINDOWS
#define DBG_CRASH(EXPRESSION,CRASHTIPS) \
    if(!(EXPRESSION)) \
    { \
        JackC::CrashHelper::GetInstance()-Crash(_CRT_WIDE(EXPRESSION),_CRT_WIDE(__FILE__),__LINE__,_CRT_WIDE(__FUNCTION__),CRASHTIPS); \
    }
#endif